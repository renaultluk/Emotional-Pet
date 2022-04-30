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

  String userName;
  bool gender;
  int age;

public:

  user();
  user(String Name, bool g, int a);
  ~user();
  
  void setGender(bool g);
  void setAge(int a);
  void setUserName(String Name);

  bool getGender();
  int getAge();
  String getUserName();

};

#endif
