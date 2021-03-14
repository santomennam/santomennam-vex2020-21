#include "main.h"
using namespace pros;
Controller master (E_CONTROLLER_MASTER);
Motor front(2);
Motor backRight(12);
Motor backLeft(19);
Motor lift(1);
Motor intakeLeft(10);
Motor intakeRight(9);
Motor roller(4);

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
class robotState
{
public:
	void rightMotors(int voltage, int reps);
	void leftMotors(int voltage, int reps);
	void forward(int voltage, int reps);
	void frontMove();
	void backward(int voltage, int reps);
	void raise(int voltage, int reps);
	void lower(int voltage,int reps);
	void intakeIn(int voltage);
	void intakeOut(int voltage);
	void brakeLeft();
	void brakeRight();
	void brakeFront();
	void rollerForward(int voltage);
	void rollerBackwards(int voltage);
	void turn(int voltageLeft, int reps);
public:
	int delay = 1;
};
//marcello, vealy, max, and rowan
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
 void stopwheels(){
    backRight.move(0);
    backLeft.move(0);
}

void setDriveVoltage(int voltL, int voltR){
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
void robotState::rightMotors(int voltage, int reps){

	backRight.move(voltage);
	for(int i =0; i<reps; i++)
	{
		pros::delay(delay);
	}
	backRight.move(0);
}
void robotState::leftMotors(int voltage, int reps)
{
	backLeft.move(voltage);
	for(int i = 0; i<reps; i++)
	{
		pros::delay(delay);
	}
	backLeft.move(0);
}
void robotState::intakeIn(int voltage)
{
		intakeLeft.move(voltage);
		intakeRight.move(-voltage);
		pros::delay(delay);
}
void robotState::intakeOut(int voltage)
{
		intakeLeft.move(-voltage);
		intakeRight.move(voltage);
		pros::delay(delay);
}
void robotState::lower(int voltage, int reps)
{
		lift.move(-voltage);
	for(int i = 0; i < reps; i++)
	{
		pros::delay(delay);
	}
	lift.move(0);
	lift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}
void robotState::raise(int voltage, int reps)
{
	lift.move(voltage);
	for(int i = 0; i < reps; i++)
	{
		pros::delay(delay);
	}
	lift.move(0);
	lift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}
void robotState::brakeFront()
{
	front.move(0);
	front.set_brake_mode(E_MOTOR_BRAKE_COAST);
}
void robotState::brakeLeft()
{
	backLeft.move(0);
	backLeft.set_brake_mode(E_MOTOR_BRAKE_COAST);
}
void robotState::brakeRight()
{
	backRight.move(0);
	backRight.set_brake_mode(E_MOTOR_BRAKE_COAST);
}
void robotState::forward(int voltage, int reps)
{
	backRight.move(-voltage);
	backLeft.move(voltage);
	frontMove();
	for(int i =0; i<reps; i++)
	{
		pros::delay(delay);
	}

	brakeLeft();
	brakeRight();
	brakeFront();
}
void robotState::backward(int voltage, int reps)
{
	backRight.move(voltage);
	backLeft.move(-voltage);
	frontMove();
	for(int i =0; i<reps; i++)
	{
		pros::delay(delay);
	}
	brakeLeft();
	brakeRight();
}
void robotState::rollerForward(int voltage)
{
		roller.move(voltage);
		pros::delay(delay);
}
void robotState::rollerBackwards(int voltage){
		roller.move(-voltage);
		pros::delay(delay);
}
void robotState::turn(int leftVoltage, int reps)
{
	backLeft.move(leftVoltage);
	backRight.move(leftVoltage);
	frontMove();
	for(int i = 0; i <reps; i++)
	{
		pros::delay(delay);
	}
	brakeLeft();
	brakeRight();
	brakeFront();
}
void frontMove()
{
		front.move((backLeft.get_voltage()+backRight.get_voltage())/2);
}
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
 void initialize() {
     pros::lcd::initialize();
     pros::lcd::set_text(1, "ChimpBot 2.0");
     pros::lcd::register_btn1_cb(on_center_button);
 //----------------------------------------------------------------------------------------------------------------------experimentalish
  // coast = will continue on momentum when button no longer pressed
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
 * for non-competition testinsg purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

	robotState robot;
// 	robot.backward(100,100);
// 	pros::delay(100);
// 	robot.forward(100,100);
// 	pros::delay(100);
// 	robot.raise(25,150);
// 	pros::delay(500);
// 	robot.forward(100,300);
// 	pros::delay(200);
// 	robot.raise(75, 1650);
// 	pros::delay(100);
// 	robot.forward(50, 800);
// 	pros::delay(100);
// 	robot.rollerBackwards(-170);
// 	pros::delay(3000);
// 	roller.move(0); //new
// 	//HERE
// 	robot.backward(50,200);
// 	pros::delay(100);
// 	robot.lower(75,1650); //owo - looks good
// 	pros::delay(100);
// //	robot.turn(100,1000); 180 degrees
// 	robot.turn(100,800);
// 	robot.rollerBackwards(170);
// 	robot.forward(127,1800);
// 	pros::delay(100);





	// robot.backward(170, 1000);
	// pros::delay(500);
	// robot.raise(170, 750);
	// robot.forward(170,100);
	// robot.intakeOut(300);
	// pros::delay(2000);
	// roller.move_voltage(0);
	// roller.set_brake_mode(MOTOR_BRAKE_HOLD);
}
//ALL VOLTAGES ARE POSITIVE!!!!!!!!!!!


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
	double intakeSpeed = 300;
	double liftSpeed = 127;
	double intakeLiftSpeed = 75;
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	controllerState controls;

	while (true) {
		controls.getControllerState();
		backLeft.move(controls.leftStick.y);
		backRight.move(-controls.rightStick.y);
		frontMove();
		// if(controls.x)
		// {
		// 	intakeLeft.move(intakeSpeed);
		// 	intakeRight.move(-intakeSpeed);
		// }
		// else{
		// 	// intakeLeft.set_brake_mode(MOTOR_BRAKE_COAST);
		// 	// intakeRight.set_brake_mode(MOTOR_BRAKE_COAST);
		// }
		// if(controls.rightBumper2)
		// {
		// 	roller.move(intakeSpeed);
		// }
		// else if(controls.rightBumper1)
		// {
		// 		roller.move(-intakeSpeed);
		// }
		// else{
		// 	roller.move(0);
		// 	roller.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		// }
		// if(controls.b)
		// {
		// 	autonomous();
		// }
		// if(controls.leftBumper1)
		// {
		// 	lift.move(liftSpeed);
		// }
		// else if(controls.leftBumper2)
		// {
		// 	lift.move(-liftSpeed);
		// }
		// else{
		// 	lift.move(0);
		// 	lift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		// }


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
