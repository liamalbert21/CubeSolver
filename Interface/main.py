import serial
import sys
import subprocess

from enum import StrEnum

# Entry point metadata
class ProgramArgs:
    COUNT = 2

    class Index:
        COM_TYPE = 1
        PORT = 2

    class ComType(StrEnum):
        AUTO = "AUTO"
        MANUAL = "MANUAL"

# Accounts for faces and directions
INSTR_PER_MOTOR = 2

# Number of motors that can run in parallel
PARALLEL_MOTOR_COUNT = 2

MIN_CMD_LENGTH = 1 + INSTR_PER_MOTOR
MAX_CMD_LENGTH = 1 + PARALLEL_MOTOR_COUNT * INSTR_PER_MOTOR

INSTR_COUNT_INDEX = 0
INSTR_SINGULAR = 'S'
INSTR_PARALLEL = 'P'

# Parallelism specifiers
SYMBOL_INSTR_TYPES = (INSTR_SINGULAR, INSTR_PARALLEL)

# Face specifiers
SYMBOL_VALID_PARALLEL_PAIRS = (('B', 'G'), ('R', 'O'), ('Y', 'W'))
SYMBOL_FACES_FLATTENED = ('R', 'O', 'Y', 'G', 'B', 'W')

# Direction specifiers
SYMBOL_DIRECTIONS = ('C', 'R', '2')

'''
Sequence of instructions (to motors)
Will eventually generalize
'''
class Command:
    QUIT = "EXIT"
    CLEAR = "CLS"

    DO_VALIDATE = 1
    DO_NOT_VALIDATE = 0

    def __init__(self, instructions: str, validate: int):
        self.instructions = instructions
        self.__hint = ""
        self.__validate = validate

    def __str__(self):
        return self.instructions

    def __isGoodLength(self, length: int, count_sym: str):
        return (length == MIN_CMD_LENGTH and count_sym == INSTR_SINGULAR or
                length == MAX_CMD_LENGTH and count_sym == INSTR_PARALLEL)

    def __isValidParallel(self, motors: tuple):
        assert len(motors) <= 2

        return (motors in SYMBOL_VALID_PARALLEL_PAIRS or
            motors[::-1] in SYMBOL_VALID_PARALLEL_PAIRS)


    # Determine whether the command's syntax is valid
    def __isValid(self):
        length = len(self.instructions)
        count_sym = self.instructions[INSTR_COUNT_INDEX]

        # Check if the instruction type was properly specified
        if count_sym not in SYMBOL_INSTR_TYPES:
            self.__hint = (
                f"Cannot deduce command type (singular/parallel)\n"
                f"Ensure the first character specifies it: {[x for x in SYMBOL_INSTR_TYPES]}"
            )
            return False

        # Determine whether the length corresponds to the parallelism instruction
        elif not self.__isGoodLength(length, count_sym):
            self.__hint = (
                f"Command insufficiently sized\n"
                f"Preserve the format: <TYPE> <FACE> <DIR> [FACE] [DIR]"
            )
            return False

        motors = []
        for i in range(INSTR_COUNT_INDEX + 1, length, INSTR_PER_MOTOR):
            face = self.instructions[i]

            # Determine whether the face and direction instructions are valid
            valid_face = face in SYMBOL_FACES_FLATTENED
            valid_dir = self.instructions[i + 1] in SYMBOL_DIRECTIONS

            if not valid_face or not valid_dir:
                error_pos = i if not valid_face else i + 1
                self.__hint = (
                    f"Invalid instruction at position {error_pos}\n"
                    f"{self.instructions}\n"
                    f"{' ' * error_pos}^\n"
                    f"{' ' * error_pos}| HERE"
                )
                return False

            motors.append(face)

        if len(motors) == PARALLEL_MOTOR_COUNT and not self.__isValidParallel(tuple(motors)):
            self.__hint = (
                f"Must ensure faces turning in parallel are opposite to one another\n"
                f"Valid pairs are: {SYMBOL_VALID_PARALLEL_PAIRS}"
            )
            return False

        return True

    # Transmit the command over UART
    def send(self, target: serial.Serial):
        if self.__validate == Command.DO_VALIDATE:
            if self.__isValid():
                print(f"Write SUCCESS\n")
                target.write(self.__str__().encode("ascii"))
            else:
                print(f"Write FAILURE\nInfo: {self.__hint}\n")
        elif self.__validate == self.DO_NOT_VALIDATE:
            target.write(self.__str__().encode("ascii"))
        else:
            print("Indecisive...")


def transmitManual(target: serial.Serial):
    status = True
    while status:
        print(f"RX: {target.read_all()}")
        cmd = Command(input("Enter command: ").replace(' ', '').upper(), Command.DO_VALIDATE)
        cmd_str = str(cmd)

        if not cmd_str:
            continue
        elif cmd_str == Command.QUIT:
            status = False
        elif cmd_str == Command.CLEAR:
            # Clear the active shell
            subprocess.run("cls" if sys.platform == "win32" else "clear", shell=True)
        else:
            cmd.send(target)


def transmitAuto(target: serial.Serial):
    print("In progress...")


def main():
    assert len(sys.argv) >= 1 + ProgramArgs.COUNT, \
        "Must pass port communication specifier (manual/auto) and port identifier (e.g. COM0)"

    target: serial.Serial
    try:
        target = serial.Serial(
            port=sys.argv[ProgramArgs.Index.PORT],
            baudrate=115200,
        )
    except serial.SerialException as e:
        print(f"ERROR: Failed to open port\nInfo: {e}")
        return

    if not target.is_open:
        target.open()

    com_type = sys.argv[ProgramArgs.Index.COM_TYPE].upper()

    match (com_type):
        case ProgramArgs.ComType.MANUAL:
            transmitManual(target)
        case ProgramArgs.ComType.AUTO:
            transmitAuto(target)
        case _:
            raise NotImplementedError("ERROR: Invalid communication specifier (manual/auto)")


if __name__ == "__main__":
    main()