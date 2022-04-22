#ifndef _USERFILE_H
#define _USERFILE_H

//constructer with parameter will create userSetting.txt under root
//constructer will read line from userSetting.txt and setup variables
//don't know how to get enum work in constructer though
//false for Female, true for male

/*format
 * userSetting
 * Female/Male
 * int represent age
 * 
*/

enum Gender {Female, Male};

class user{

  bool gender;
  int age;

public:

  user();
  user(bool g, int a);
  void setGender(bool g);
  void setAge(int a);

  bool getGender();
  int getAge();

};

#endif
