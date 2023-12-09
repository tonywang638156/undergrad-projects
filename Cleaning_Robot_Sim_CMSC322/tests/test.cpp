// # include "robot.cpp"
# include "RMS.cpp"
# include <iostream>
using namespace std;

int main(){
    // Test Robot Class
    Robot* r1;
    Robot* r2;
    Robot* r3;
    Robot* r4;

    r1 = new Robot(Size::Small, Type::Mop, 0, "hub");
    r2 = new Robot(Size::Medium, Type::Vacuum, 0, "hub");
    r3 = new Robot(Size::Large, Type::Sweeper, 0, "hub");
    r4 = new Robot(Size::Small, Type::Mop, 0, "hub");

    r1->print();
    r2->print();
    r3->print();
    r4->print();
    
    // Test RMS
    RobotManagementSystem rms;
    rms.addRobot(1, r1);
    rms.addRobot(2, r2);
    rms.addRobot(3, r3);
    rms.addRobot(4, r4);

    cout << rms.requestCleaning(Size::Large, Type::Vacuum) << endl;
    cout << rms.requestCleaning(Size::Small, Type::Mop) << endl;
    
    return 0;
}