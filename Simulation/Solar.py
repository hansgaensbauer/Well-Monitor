import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import pvlib
from dataclasses import dataclass

#dataclass for holding state variables. If you want to track a new variable
#by accumulating behavior, add it here
@dataclass
class SysState:
    power: float
    time: float
    tank_level: float
    grid_power: float
    pump_rate: float
    use_rate: float

##################################################################
######################## System Behaviors ########################
##################################################################

class SysBehavior:
    #Most basic model: constant pump rate.
    def constant_rate(sys):
        sys.state = SysState(
            sys.state.power, 
            sys.state.time + 1, 
            sys.state.tank_level + 10 * 60 - sys.state.use_rate, 
            sys.state.grid_power, 
            10* 60,
            sys.state.use_rate
            )
    
    #Pump at a constant rate during a certain time window (set to 6am to Noon)
    def constant_rate_time_window(sys):
        rate = (sys.state.time % 24 > 12 and sys.state.time  % 24 < 18) * 10  * 60
        sys.state = SysState(
            sys.state.power, 
            sys.state.time + 1, 
            sys.state.tank_level + rate - sys.state.use_rate, 
            sys.state.grid_power, 
            rate, 
            sys.state.use_rate
            )

    #Fit curve to plot in datasheet
    def only_solar(sys):
        exp = 0.25
        xofs = -500
        r1 = (33.5 - 7.9523)
        r2 = (np.power(3203.16 + xofs, exp) - np.power(961.708 + xofs, exp))
        ofs = np.power(961.708 + xofs, exp) * r1/r2
        interpolated = np.power(np.maximum(1, sys.state.power + xofs), exp) * r1/r2 + 7.9523 - ofs
        rate = np.minimum(interpolated * (sys.state.power > 961.708), 33.5) * 60
        sys.state = SysState(
            sys.state.power, 
            sys.state.time + 1, 
            sys.state.tank_level + rate - sys.state.use_rate, 
            sys.state.grid_power, 
            rate, 
            sys.state.use_rate
            )

    #Use solar for a while, then switch to grid
    def solar_grid_time_window(sys):

        grid_switch_time  = 17 #5:00pm
        grid_off_time = 18 #6:00pm

        rate = 0
        grid_power = 0

        if (((sys.state.time - 6) % 24) >= grid_switch_time and ((sys.state.time - 6) % 24) < grid_off_time):
            rate = 33.5 * 60
            grid_power = 3203.16
        else:
            exp = 0.25
            xofs = -500
            r1 = (33.5 - 7.9523)
            r2 = (np.power(3203.16 + xofs, exp) - np.power(961.708 + xofs, exp))
            ofs = np.power(961.708 + xofs, exp) * r1/r2
            interpolated = np.power(np.maximum(1, sys.state.power + xofs), exp) * r1/r2 + 7.9523 - ofs
            rate = np.minimum(interpolated * (sys.state.power > 961.708), 33.5) * 60

        if(sys.state.tank_level >= (sys.waterlevel_max - rate + sys.state.use_rate)):
            rate_init = rate
            rate = np.minimum(sys.state.use_rate + (sys.waterlevel_max - sys.state.tank_level), rate) #pump only what will keep the tank full
            grid_power = grid_power * rate / rate_init #we only need energy for when we were pumping

        sys.state = SysState(
            sys.state.power, 
            sys.state.time + 1, 
            sys.state.tank_level + rate - sys.state.use_rate, 
            sys.state.grid_power + grid_power, #full power while grid is on
            rate, 
            sys.state.use_rate
            )

##################################################################
###################### Water System Object #######################
##################################################################

class WaterSystem:
    #class variables
    def __init__(self, behav, initial_state, waterlevel_max, panel, numpanels):
        self.step_behavior = behav
        self.numpanels = numpanels
        self.state = initial_state
        self.waterlevel_max = waterlevel_max
        self.panel = panel

    # Model conversion losses in the system
    def calc_loss(self, power_array):
        for index, row in power_array.iterrows():
            row['p_mp'] = 0.8 * row['p_mp'] #80 percent conversion efficiency

    #takes a current water use rate and a current solar power, and modifies self.state to get the next state
    def step(self, use_rate, power):
        self.state.use_rate = use_rate
        self.state.power = power * self.numpanels
        self.step_behavior(self)
        self.state.tank_level = np.minimum(np.maximum(0, self.state.tank_level), self.waterlevel_max)

##################################################################
######################### PV Simulation ##########################
##################################################################

#dataclass with parameters from solar panel datasheet
@dataclass
class PV_Params:
    length: int
    width: int
    celltype: str
    STC: float
    PTC: float
    V_MP: float
    I_MP: float
    V_OC: float
    I_SC: float
    ALPHA_SC: float
    BETA_VOC: float
    GAMMA_PMP: float
    NSERIES: int
    TEMP_REF: float

#Solar Panel Object
class PV_Panel:

    def __init__(self, inclination, module_data):
        self.module_data = module_data
        self.inclination = inclination
        self.module = pvlib.pvsystem.retrieve_sam('SandiaMod')['Canadian_Solar_CS5P_220M___2009_']
        self.temperature_model_parameters = pvlib.temperature.TEMPERATURE_MODEL_PARAMETERS['sapm']['open_rack_glass_glass']

        #Set up a custom solar module using module_data
        self.cec_fit_params = pvlib.ivtools.sdm.fit_cec_sam(module_data.celltype, module_data.V_MP, module_data.I_MP,
                            module_data.V_OC, module_data.I_SC, module_data.ALPHA_SC,
                            module_data.BETA_VOC, module_data.GAMMA_PMP, 
                            module_data.NSERIES, module_data.TEMP_REF)

         # print(cec_fit_params)

        self.system = {'module': self.module,
          'surface_azimuth': 0, 'surface_tilt': self.inclination}

        self.energies = {}

        # self.weather = pvlib.iotools.get_pvgis_tmy(self.latitude, self.longitude, map_variables=True)[0]

    def power_calc(self, weather, latitude, longitude, altitude):

        weather.index.name = "utc_time"

        solpos = pvlib.solarposition.get_solarposition(
            time = weather.index, 
            latitude = latitude, 
            longitude = longitude,
            altitude = altitude,
            temperature = weather["temp_air"],
            pressure = pvlib.atmosphere.alt2pres(altitude),
        )

        dni_extra = pvlib.irradiance.get_extra_radiation(weather.index)

        pressure = pvlib.atmosphere.alt2pres(altitude)
        airmass = pvlib.atmosphere.get_relative_airmass(solpos['apparent_zenith'])
        am_abs = pvlib.atmosphere.get_absolute_airmass(airmass, pressure)

        aoi = pvlib.irradiance.aoi(
            self.system['surface_tilt'], 
            self.system['surface_azimuth'],
            solpos['apparent_zenith'], 
            solpos['azimuth'],
        )

        total_irrad = pvlib.irradiance.get_total_irradiance(self.system['surface_tilt'],
                                                            self.system['surface_azimuth'],
                                                            solpos['apparent_zenith'],
                                                            solpos['azimuth'],
                                                            weather['dni'], 
                                                            weather['ghi'], 
                                                            weather['dhi'],
                                                            dni_extra=dni_extra,
                                                            model='haydavies',
                                                            )

        cell_temperature = pvlib.temperature.sapm_cell(
            total_irrad['poa_global'],
            weather["temp_air"],
            weather["wind_speed"],
            **self.temperature_model_parameters,
        )

        effective_irradiance = pvlib.pvsystem.sapm_effective_irradiance(
            total_irrad['poa_direct'], 
            total_irrad['poa_diffuse'],
            am_abs, 
            aoi, 
            self.module,
        )
        diode_params = pvlib.pvsystem.calcparams_cec(effective_irradiance, cell_temperature, self.module_data.ALPHA_SC, self.cec_fit_params[4], 
                                            self.cec_fit_params[0], self.cec_fit_params[1], self.cec_fit_params[3], 
                                            self.cec_fit_params[2], self.cec_fit_params[5])

        # Estimate I-V characteristic using the Single Diode Equation
        iv_values = pvlib.pvsystem.singlediode(diode_params[0], 
                                            diode_params[1], 
                                            diode_params[2], 
                                            diode_params[3], 
                                            diode_params[4], 
                                            ivcurve_pnts=0,   # Not interested in IV curve
                                            method='lambertw') # I-V using the Lambert W. function

        return iv_values