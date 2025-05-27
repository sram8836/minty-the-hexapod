from servo import servo2040, ServoCluster, Calibration, Servo
import select
import sys
from time import sleep

num_servos = 18
cluster_size = 18
test_all = 0
test_one = 0

cal = []

for _ in range(18):
    cal.append(Calibration())

cal[0].apply_two_pairs(1020, 2020, -45, 45), # 1: L31
cal[1].apply_two_pairs(1000, 2000, -45, 45), # 2: L32
cal[2].apply_two_pairs(970, 1980, -45, 45), # 3: L33
cal[3].apply_two_pairs(1000, 2070, -45, 45), # 4: R33
cal[4].apply_two_pairs(890, 1900, -45, 45), # 5: R32
cal[5].apply_two_pairs(980, 2060, -45, 45), # 6: R31
cal[6].apply_two_pairs(970, 2030, -45, 45), # 7: L21
cal[7].apply_two_pairs(1020, 2030, -45, 45), # 8: L22
cal[8].apply_two_pairs(920, 1960, -45, 45), # 9: L23
cal[9].apply_two_pairs(1040, 1990, -45, 45), # 10: R23
cal[10].apply_two_pairs(1000, 2000, -45, 45), # 11: R22
cal[11].apply_two_pairs(910, 1930, -45, 45), # 12: R21
cal[12].apply_two_pairs(890, 1900, -45, 45), # 13: L11
cal[13].apply_two_pairs(880, 1930, -45, 45), # 14: L12
cal[14].apply_two_pairs(1000, 2020, -45, 45), # 15: L13
cal[15].apply_two_pairs(910, 1900, -45, 45), # 16: R13
cal[16].apply_two_pairs(1030, 2000, -45, 45), # 17: R11
cal[17].apply_two_pairs(1060, 2020, -45, 45), # 18: R12

# Reversed joints
cal[0].apply_two_pairs(2020, 1050, -45, 45), # 1: L31
cal[1].apply_two_pairs(2020, 1020, -45, 45), # 2: L32
cal[2].apply_two_pairs(1980, 970, -45, 45), # 3: L33
cal[5].apply_two_pairs(2060, 980, -45, 45), # 6: R31 # Coxa
cal[8].apply_two_pairs(1960, 920, -45, 45), # 9: L23 # Coxa
cal[9].apply_two_pairs(1990, 960, -45, 45), # 10: R23
cal[10].apply_two_pairs(2000, 950, -45, 45), # 11: R22
cal[11].apply_two_pairs(1930, 910, -45, 45), # 12: R21
cal[12].apply_two_pairs(1900, 890, -45, 45), # 13: L11
cal[13].apply_two_pairs(1950, 930, -45, 45), # 14: L12
cal[14].apply_two_pairs(2020, 1000, -45, 45), # 15: L13
cal[17].apply_two_pairs(2020, 1060, -45, 45), # 18: R12 # Coxa


def parse_angles(command: str) -> list[float]:
    if not command.startswith("angles:"):
        return None
    
    angles = command[7:] # Remove prefix
    
    try:
        values = [float(num) for num in angles.split(';') if num]
    except:
        return None

    return values

def main():
    # Create servo cluster objects
    pins = [None]
    cluster = [None]
    pins[0] = list(range(servo2040.SERVO_1, servo2040.SERVO_18 + 1))
    cluster[0] = ServoCluster(1, 0, pins[0])
    
    # Apply servo calibration
    for i in range(num_servos):
        cluster_index = i // cluster_size
        servo_num = i % cluster_size
        cal[i].limit_to_calibration(False, False)
        cluster[cluster_index].calibration(servo_num, cal[i])
    
    # Go to home position
    for i in range (num_servos):
        cluster_index = i // cluster_size
        servo_num = i % cluster_size
        cluster[cluster_index].value(servo_num, 0, load=True)
        
    print("Calibration complete")
    
    while test_all:
        for i in range (num_servos):
            cluster_index = i // cluster_size
            servo_num = i % cluster_size
            joint_type = i % 3
                
            if joint_type == 0: # Tibia
                cluster[cluster_index].value(servo_num, -60, load=True)
            elif joint_type == 1: # Femur
                cluster[cluster_index].value(servo_num, -90, load=True)
            else: # Coxa
                cluster[cluster_index].value(servo_num, 0, load=True)
                
            print("Servo", i, ":", cluster[cluster_index].pulse(servo_num))
        sleep(10.0)
    
    while test_one:    
        i = 8
        cluster_index = i // cluster_size
        servo_num = i % cluster_size
        
        while True:
            for angle in range(30, -31, -5):
                cluster[cluster_index].value(servo_num, angle, load=True)
                print(angle)
                sleep(0.1)
            for angle in range(-31, 30, 5):
                cluster[cluster_index].value(servo_num, angle, load=True)
                print(angle)
                sleep(0.1)
        
    
    print("Ready")
    
    # Listen for commands
    while True:
        if sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
            cmd = sys.stdin.readline().strip()

            # Parse command
            angles = parse_angles(cmd)

            # Check parse successful
            if not angles:
                print("Ignoring malformed serial")
                continue
            
            # Clip out of range values
            for i in range(len(angles)):
                joint_type = i % 3
                
                if joint_type == 0: # Tibia
                    if angles[i] < -90:
                        angles[i] = -90
                    elif angles[i] > 65:
                        angles[i] = 65
                elif joint_type == 1: # Femur
                    if angles[i] < -90:
                        angles[i] = -90
                    elif angles[i] > 60:
                        angles[i] = 60
                else: # Coxa
                    if angles[i] < -30:
                        angles[i] = -30
                    elif angles[i] > 30:
                        angles[i] = 30
            
            # Observe command
            for i, angle in enumerate(angles):
                cluster_index = i // cluster_size
                servo_num = i % cluster_size
                cluster[cluster_index].value(servo_num, angle, load=True)


if __name__=="__main__":
    main()
