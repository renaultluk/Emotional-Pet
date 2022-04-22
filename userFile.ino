#include "userFile.h"
#include "SDcard.h"

user::user(){
  String genderString = readFileline(SD, "/userSetting.txt", 2);
  String ageString = readFileline(SD, "/userSetting.txt", 3);
  
  if (genderString == " Female"){gender = false;}
  else {gender = true;}
  age = ageString.toInt();
}

user::user(bool g, int a){
  
  gender = g;
  age = a;
  // with new user data, old userSetting deleted and new userSetting write
  deleteFile(SD, "/userSetting.txt");
  writeFile(SD, "/userSetting.txt", "userSetting\n ");
  if (g){appendFile(SD, "/userSetting.txt", "Male\n");}
  else{appendFile(SD, "/userSetting.txt", "Female\n");}
  char age[2];
  itoa(a, age, 10);
  Serial.print(age);
  appendFile(SD, "/userSetting.txt", age);
  
}

void user::setGender(bool g){
  gender = g;
  Serial.printf("gender set %d\n", g);
  };
void user::setAge(int a){
  age = a;
  Serial.printf("age set %d\n",a);
  };

bool user::getGender(){
  return gender;
  };
int user::getAge(){
  return age;
  };
