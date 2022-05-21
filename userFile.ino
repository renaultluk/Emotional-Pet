#include "userFile.h"
#include "SDcard.h"

// The constructor read the data from txt file and create user object
user::user(){
  userName = readFileline(SD, "/userSetting.txt", 2);
  String genderString = readFileline(SD, "/userSetting.txt", 3);
  String ageString = readFileline(SD, "/userSetting.txt", 4);

  if (genderString == " Female"){gender = false;}
  else {gender = true;}
  age = ageString.toInt();
}

// The constructor read create userSetting.txt, put in the userdata and create user object
user::user(String Name, bool g, int a){

  userName = Name;
  gender = g;
  age = a;
  // with new user data, old userSetting deleted and new userSetting overwrite
  deleteFile(SD, "/userSetting.txt");
  writeFile(SD, "/userSetting.txt", "userSetting\n");
  
  appendFile(SD, "/userSetting.txt", Name.c_str());
  
  if (g){appendFile(SD, "/userSetting.txt", "\nMale\n");}
  else{appendFile(SD, "/userSetting.txt", "\nFemale\n");}
  char age[2];
  itoa(a, age, 10);
  appendFile(SD, "/userSetting.txt", age);
}

user::~user(){
  
  deleteFile(SD, "/userSetting.txt");
  writeFile(SD, "/userSetting.txt", "userSetting\n");
  appendFile(SD, "/userSetting.txt", userName.c_str());
  if (gender){appendFile(SD, "/userSetting.txt", "\nMale\n");}
  else{appendFile(SD, "/userSetting.txt", "\nFemale\n");}
  char temp[2];
  itoa(age, temp, 10);
  appendFile(SD, "/userSetting.txt", temp);
  };

void user::setGender(bool g){
  gender = g;
  Serial.printf("gender set %d\n", g);
  };
  
void user::setAge(int a){
  age = a;
  Serial.printf("age set %d\n",a);
  };

void user::setUserName(String Name){
  userName = Name;
  Serial.print("userName set" );
  Serial.println(Name);
}


bool user::getGender(){
  return gender;
  };
int user::getAge(){
  return age;
  };
String user::getUserName(){
  return userName;
}
