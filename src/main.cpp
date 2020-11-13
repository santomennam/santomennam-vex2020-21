#include "main.h"
using namespace pros;
Controller master (E_CONTROLLER_MASTER);
Motor frontRight(14);
Motor frontLeft(11);
Motor backRight(13);
Motor backLeft(12);
Motor lift(17);
Motor intakeLeft(4);
Motor intakeRight(6);
Motor roller(7);
class robotState
{
public:
	robotState();
	void rightMotors(int voltage, int reps);
	void leftMotors(int voltage, int reps);
	void forward(int voltage, int reps);
	void backward(int voltage, int reps);
	void raise(int voltage, int reps);
	void lower(int voltage, int reps);
	void intakeIn(int voltage, int reps);
	void intakeOut(int voltage, int reps);
	void brakeLeft();
	void brakeRight();
	void rollerForward(int voltage, int reps);
	void rollerBackwards(int voltage, int reps);
public:
	int delay = 10;
};
class Vec2d
{
public:
	int x;
	int y;
public:
	Vec2d(int one,int two);
};
Vec2d::Vec2d(int one, int two)
{
	x = one;
	y = two;
}
class controllerState
{
public:
	void getControllerState();
	void drive();
	controllerState();
public:
	bool rightBumper1;
  bool leftBumper1;
  bool rightBumper2;
  bool leftBumper2;
	Vec2d leftStick;
	Vec2d rightStick;
	bool a;
	bool b;
	bool y;
	bool x;
	bool larrow;
	bool rarrow;
	bool uarrow;
	bool darrow;
};
void controllerState::getControllerState()
{
	rightBumper1 = master.get_digital(DIGITAL_R1);
	leftBumper1 = master.get_digital(DIGITAL_L1);
	rightBumper2 = master.get_digital(DIGITAL_R2);
	leftBumper2 = master.get_digital(DIGITAL_L2);
	 a = master.get_digital_new_press(DIGITAL_A);
	 b = master.get_digital_new_press(DIGITAL_B);
	 y = master.get_digital_new_press(DIGITAL_Y);
	 x = master.get_digital_new_press(DIGITAL_X);
	 rarrow = master.get_digital(DIGITAL_RIGHT);
	 uarrow = master.get_digital(DIGITAL_UP);
	 darrow = master.get_digital(DIGITAL_DOWN);
	 larrow = master.get_digital(DIGITAL_LEFT);
	 leftStick.y = master.get_analog(ANALOG_LEFT_Y);
	 leftStick.x = master.get_analog(ANALOG_LEFT_X);
	 rightStick.y = master.get_analog(ANALOG_RIGHT_Y);
	 rightStick.x = master.get_analog(ANALOG_RIGHT_X);
}
controllerState::controllerState()
: leftStick(0,0), rightStick(0,0)
{
	rightBumper1 =0;
	leftBumper1 = 0;
	rightBumper2 =0;
	leftBumper2 = 0;
	 a = 0;
	 b = 0;
	 y =0;
	 x =0;
	 rarrow =0;
	 uarrow = 0;
	 darrow =0;
	 larrow =0;
}
//marcello, vealy, max, and rowan
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
 void stopwheels(){
    frontLeft.move(0);
    frontRight.move(0);
    backRight.move(0);
    backLeft.move(0);
}

void setDriveVoltage(int voltL, int voltR){
	frontLeft=voltL;
	frontRight=-voltR;
	backRight=-voltR;
	backLeft=voltL;
}
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "Monkey Mode");
	} else {
		pros::lcd::clear_line(2);
	}
}
robotState::robotState()
{
	delay = 10;
}
void robotState::rightMotors(int voltage, int reps)
{
	for(int i =0; i<reps; i++)
	{
		frontRight.move(voltage);
		backRight.move(voltage);
		pros::delay(delay);
	}

}
void robotState::leftMotors(int voltage, int reps)
{
	for(int i = 0; i<reps; i++)
	{
	frontLeft.move(voltage);
	backLeft.move(voltage);
	pros::delay(delay);
}

}
void robotState::intakeIn(int voltage, int reps)
{
	for(int i = 0; i < reps; i++)
	{
		intakeLeft.move(voltage);
		intakeRight.move(-voltage);
		pros::delay(delay);
	}

}
void robotState::intakeOut(int voltage, int reps)
{
	for(int i = 0; i < reps; i++)
	{
		intakeLeft.move(-voltage);
		intakeRight.move(voltage);
		pros::delay(delay);
	}

}
void robotState::lower(int voltage, int reps)
{
	for(int i = 0; i < reps; i++)
	{
		lift.move(-voltage);
		pros::delay(delay);
	}
	lift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}
void robotState::raise(int voltage, int reps)
{
	for(int i = 0; i < reps; i++)
	{
		pros::delay(delay);
	}

	lift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}
void robotState::brakeLeft()
{
	backLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	frontLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}
void robotState::brakeRight()
{
	backRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	frontRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}
void robotState::forward(int voltage, int reps)
{

		rightMotors(voltage, reps);
		leftMotors(voltage, reps);
		pros::delay(delay);

	brakeLeft();
	brakeRight();

}
void robotState::backward(int voltage, int reps)
{
	rightMotors(-voltage, reps);
	leftMotors(-voltage, reps);
		pros::delay(delay);
	brakeLeft();
	brakeRight();
}
void robotState::rollerForward(int voltage, int reps)
{
	for(int i = 0; i < reps; i++)
	{
		roller.move(voltage);
		pros::delay(delay);
	}
}
void robotState::rollerBackwards(int voltage, int reps)
{
	for(int i = 0; i < reps; i++)
	{
		roller.move(-voltage);
		pros::delay(delay);
	}
}
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
 void initialize() {
     pros::lcd::initialize();
     pros::lcd::set_text(1, "Gamer Time");
     pros::lcd::register_btn1_cb(on_center_button);
 //----------------------------------------------------------------------------------------------------------------------experimentalish
     frontLeft.set_brake_mode(E_MOTOR_BRAKE_COAST);//set_brake_mode(E_MOTOR_BRAKE_COAST); -- for drive motors
     frontRight.set_brake_mode(E_MOTOR_BRAKE_COAST); // coast = will continue on momentum when button no longer pressed
     backLeft.set_brake_mode(E_MOTOR_BRAKE_COAST);
     backRight.set_brake_mode(E_MOTOR_BRAKE_COAST);
		 opcontrol;
 }

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}
//ALL VOLTAGES ARE POSITIVE!!!!!!!!!!!
robotState vexBOT();
vexBOT.rollerBackwards(10,10);

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	double intakeSpeed = 127;
	double liftSpeed = 127;
	double intakeLiftSpeed = 75;
		pros::Controller master(pros::E_CONTROLLER_MASTER);
	controllerState controls;

	while (true) {
		controls.getControllerState();
		frontLeft.move(controls.leftStick.y);
		backLeft.move(controls.leftStick.y);
		frontRight.move(-controls.rightStick.y);
		backRight.move(-controls.rightStick.y);
		if(controls.rightBumper1)
		{
			intakeLeft.move(intakeSpeed);
			intakeRight.move(-intakeSpeed);
		}
		else if(controls.rightBumper2)
		{
			intakeLeft.move(-intakeSpeed);
			intakeRight.move(intakeSpeed);
		}
		else{
			intakeLeft.move(0);
			intakeRight.move(0);
			intakeRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
			intakeLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		}

		if(controls.leftBumper1)
		{
			lift.move(liftSpeed);
		}
		else if(controls.leftBumper2)
		{
			lift.move(-liftSpeed);
		}
		else{
			lift.move(0);
			lift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		}


		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		pros::delay(10);
	}
}
int main()
{
	opcontrol;
	return 0;
}
