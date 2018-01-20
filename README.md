# Two-wheeled_Self-balance

* ## Variables
            Angle_err       Deflection angle
            Angle_Gyro      Angle speed
            Moto_Speed_L    Speed of left moto
            Moto_Speed_R    Speed of right moto
            Moto_Disp_L     Displacement of left moto
            Moto_Disp_R     Displacement of right moto


* ## Core Algorithm
            PWM_L = (Kp_1 * Angle_err + Kd_1 * Angle_Gyro) + (Kp_2 * Moto_Disp_L + Kd_2 * Moto_Disp_L)
            PWM_R = (Kp_1 * Angle_err + Kd_1 * Angle_Gyro) + (Kp_2 * Moto_Disp_R + Kd_2 * Moto_Disp_R)
  
* ## Hardware
            1. TB6612FNG  (Moto Controller)
              PWM_B	-> PC7	
              AIN_1	-> PB14	
              AIN_2	-> PB15	
              PWM_A	-> PC6		
              NC      -> PA8	
              BIN_1	-> PC10	
              BIN_2	-> PC11	

            2. hc06    (Slave Bluetooth)
              RX  -> PA.2
              TX  -> PA.3
            3. hc05    (Host Bluetooth)
              RX  -> PA.2
              TX  -> PA.3
            4. Keys
              up    -> PB3
              down  -> PB5
              left  -> PB7
              right -> PB4			
