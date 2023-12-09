#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cctype>

using namespace std;

void SplitString(string s, vector<string> &v);
unordered_map<string,int> createHashMap(unordered_map<string,int> map);
unordered_map<string, int> map;
unordered_map<string, int> map_instr; //map of labels


int j(vector<string> &v);
int jal(vector<string> &v);
int jr(vector<string> &v);
int jalr(vector<string> &v);
int beq(int cur_line, vector<string> &v);
int bne(int cur_line, vector<string> &v);
int addi(vector<string> &v);
int sub_add_slt(vector<string> &v);
int mult_div(vector<string> &v);
int mflo_mfhi(vector<string> &v);
int sll_srl( vector<string> &v);
int lw_sw(vector<string> &v);
int syscall(vector<string> &v);
int clo_clz(vector<string> &v);

void strip(std::string& str);

int main(int argc, char* argv[]) {
   ofstream outputfile;
   outputfile.open(argv[argc-1],ios::binary); //Open file in binary mode
   for (int file_index = 0; file_index < argc - 1; file_index++){
      string filename = argv[file_index];
      ifstream file(filename);
      string str;
      vector<string> array;
      int index = 0;

      while (getline(file, str)) {
            strip(str);
            for(int i = 0; i < str.size(); i++){
               if (str[i] == '#'){ // get rid of all comments, first pass
                  str = str.substr(0, i);
                  break;
               }
            }
         if(str.empty()){  // skip the line when it's empty line
            continue;
         }
         str.erase(remove(str.begin(), str.end(), ','), str.end());
         string firstWord = str.substr(0, str.find(" "));
         // check whether it's label or not
         if (firstWord != "add" && firstWord != "addi" && firstWord != "sub"
            && firstWord != "mult" && firstWord != "div" && firstWord != "mflo"
            && firstWord != "mfhi" && firstWord != "srl" && firstWord != "sll"
            && firstWord != "lw" && firstWord != "sw" && firstWord != "slt"
            && firstWord != "beq" && firstWord != "bne" && firstWord != "j"
            && firstWord != "jal" && firstWord != "jr" && firstWord != "jalr"
            && firstWord != "syscall" && firstWord != "sgt" && firstWord != "sge"
            && firstWord != "sle" && firstWord != "sne" && firstWord != "seq"
            && firstWord != "clo" && firstWord != "clz"){
               firstWord.erase(remove(firstWord.begin(), firstWord.end(), ':'), firstWord.end());
               map_instr[firstWord] = index; // store the line number into hashmap
         } else {
            array.push_back(str); // put each instruction into the vector
            index++;
         }
      }

      map = createHashMap(map);

      for (int i = 0; i < array.size(); i++){
         vector<string> strvec;
         // get each part of token from each instruction and store it into vector
         SplitString(array.at(i), strvec);
         // based on the first part of instruction to determine which function to call
         if (strvec[0] == "add" || strvec[0] == "sub" || strvec[0] == "slt") {
            int res = (sub_add_slt(strvec));
            outputfile.write((char *) &res, sizeof(int)); // output the number into the bin file
         } else if(strvec[0] == "addi") {
            int imm = stoi(strvec[3]);
            int res = 0;
            if ((imm >> 16) > 0){
                string reg1 = strvec[1];
                string reg2 = strvec[2];
                vector<string> help_vector;
                help_vector.push_back("addi");
                help_vector.push_back(reg1);
                help_vector.push_back("$0");
                int first_half = (imm >> 16);
                help_vector.push_back(to_string(first_half));
                res = (addi(help_vector));
                outputfile.write((char *) &res, sizeof(int));
                res = 0;
                help_vector.clear();
                help_vector.push_back("sll");
                help_vector.push_back(reg1);
                help_vector.push_back(reg1);
                help_vector.push_back(to_string(16));
                res = (sll_srl(help_vector));
                outputfile.write((char *) &res, sizeof(int));
                help_vector.clear();
                res = 0;
                help_vector.push_back("addi");
                help_vector.push_back(reg1);
                help_vector.push_back(reg1);
                int second_half = (imm % (1 << 16));
                help_vector.push_back(to_string(second_half));
                res = (addi(help_vector));
                outputfile.write((char *) &res, sizeof(int));
                res = 0;
                help_vector.clear();
                help_vector.push_back("add");
                help_vector.push_back(reg1);
                help_vector.push_back(reg1);
                help_vector.push_back(reg2);
                res = (sub_add_slt(help_vector));
                outputfile.write((char *) &res, sizeof(int));
                res = 0;
                help_vector.clear();
            }
            else{
                res = (addi(strvec));
                outputfile.write((char *) &res, sizeof(int));
            }
         } else if(strvec[0] == "mult" || strvec[0] == "div") {
            int res = (mult_div(strvec));
            outputfile.write((char *) &res, sizeof(int));
         } else if(strvec[0] == "mflo" || strvec[0] == "mfhi") {
            int res = (mflo_mfhi(strvec));
            outputfile.write((char *) &res, sizeof(int));
         } else if(strvec[0] == "sll" || strvec[0] == "srl") {
            int res = (sll_srl(strvec));
            outputfile.write((char *) &res, sizeof(int));
         } else if(strvec[0] == "lw" || strvec[0] == "sw") {
            int res = (lw_sw(strvec));
            outputfile.write((char *) &res, sizeof(int));
         } else if(strvec[0] == "syscall") {
            int res = (syscall(strvec));
            outputfile.write((char *) &res, sizeof(int));
         } else if (strvec[0] == "j") {
            int res = (j(strvec));
            outputfile.write((char *) &res, sizeof(int));
         } else if (strvec[0] == "jal") {
            int res = (jal(strvec));
            outputfile.write((char *) &res, sizeof(int));
         } else if (strvec[0] == "jalr") {
            int res = (jalr(strvec));
            outputfile.write((char *) &res, sizeof(int));
         } else if (strvec[0] == "jr") {
            int res = (jr(strvec));
            outputfile.write((char *) &res, sizeof(int));
         } else if (strvec[0] == "beq") {
            int res = beq(i, strvec);
            outputfile.write((char *) &res, sizeof(int));
         } else if (strvec[0] == "bne") {
            int res = bne(i, strvec);
            outputfile.write((char *) &res, sizeof(int));
        } else if(strvec[0] == "clo" || strvec[0] == "clz"){
            int res = clo_clz(strvec);
            outputfile.write((char *) &res, sizeof(int));
        } else if (strvec[0] == "sgt"){
            vector<string> help_vector;
            help_vector.push_back("slt");
            help_vector.push_back(strvec[1]);
            help_vector.push_back(strvec[3]);
            help_vector.push_back(strvec[2]);
            int res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
        } else if (strvec[0] == "sge"){
            vector<string> help_vector;
            help_vector.push_back("slt");
            help_vector.push_back(strvec[1]);
            help_vector.push_back(strvec[2]);
            help_vector.push_back(strvec[3]);
            int res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("addi");
            help_vector.push_back("$t7");
            help_vector.push_back("$0");
            help_vector.push_back("1");
            res = addi(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("sub");
            help_vector.push_back(strvec[1]);
            help_vector.push_back("$t7");
            help_vector.push_back(strvec[1]);
            res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
        } else if (strvec[0] == "sle"){
            vector<string> help_vector;
            help_vector.push_back("slt");
            help_vector.push_back(strvec[1]);
            help_vector.push_back(strvec[3]);
            help_vector.push_back(strvec[2]);
            int res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("addi");
            help_vector.push_back("$t7");
            help_vector.push_back("$0");
            help_vector.push_back("1");
            res = addi(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("sub");
            help_vector.push_back(strvec[1]);
            help_vector.push_back("$t7");
            help_vector.push_back(strvec[1]);
            res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
        } else if (strvec[0] == "sne"){
            vector<string> help_vector;
            help_vector.push_back("slt");
            help_vector.push_back(strvec[1]);
            help_vector.push_back(strvec[2]);
            help_vector.push_back(strvec[3]);
            int res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("slt");
            help_vector.push_back("$t7");
            help_vector.push_back(strvec[3]);
            help_vector.push_back(strvec[2]);
            res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("add");
            help_vector.push_back(strvec[1]);
            help_vector.push_back("$t7");
            help_vector.push_back(strvec[1]);
            res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
        } else if (strvec[0] == "seq"){
            vector<string> help_vector;
            help_vector.push_back("slt");
            help_vector.push_back(strvec[1]);
            help_vector.push_back(strvec[2]);
            help_vector.push_back(strvec[3]);
            int res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("slt");
            help_vector.push_back("$t7");
            help_vector.push_back(strvec[3]);
            help_vector.push_back(strvec[2]);
            res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("add");
            help_vector.push_back(strvec[1]);
            help_vector.push_back("$t7");
            help_vector.push_back(strvec[1]);
            res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("addi");
            help_vector.push_back("$t7");
            help_vector.push_back("$0");
            help_vector.push_back("1");
            res = addi(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
            help_vector.push_back("sub");
            help_vector.push_back(strvec[1]);
            help_vector.push_back("$t7");
            help_vector.push_back(strvec[1]);
            res = sub_add_slt(help_vector);
            outputfile.write((char *) &res, sizeof(int));
            help_vector.clear();
            res = 0;
        }
      }
   }
   outputfile.close();
}

// addi with format: addi $t0 $t1 consti
int addi(vector<string> &v) {
   int res = map[v[0]];
   res = res << 5;
   res += map[v[2]]; // switch 2nd register place and 1st register place
   res = res << 5;
   res += map[v[1]]; // append 1st register
   if (stoi(v[3]) < 0) {
        res += 1; // handle the negative case for addi
   }
   res = res << 16;
   res+=stoi(v[3]);
   return res;
}

// add ,slt and sub with format: sub t0 t1 t2
int sub_add_slt(vector<string> &v) {
   int res = map[v[2]]; //get 2nd register (t1) first
   res = res << 5;
   res += map[v[3]]; //get 3rd register (t2) first
   res = res << 5;
   res += map[v[1]]; //get 1st register (t0) first
   res = res << 5; // shift left 5 bits for empty space
   res = res << 6; // shift left 6 bits for add or sub instruction code
   res += map[v[0]];
   return res;
}

// mult and div with format: mult t0 t1
int mult_div(vector<string> &v) {
  int res = map[v[1]]; //get t0
  res = res << 5;
  res += map[v[2]]; //get t1
  res = res << 10; // shift left 10 bits for empty space
  res = res << 6; // shift left 6 bits for mult or div instruction code
  res += map[v[0]];
  return res;
}

// mflo and mfhi with format: mflo t0
int mflo_mfhi(vector<string> &v) {
  int res = map[v[1]]; //get t0
  res = res << 5; // shift left 5 bits for empty space
  res = res << 6; // shift left 6 bits for mfhi or mflo instruction code
  res += map[v[0]];
  return res;
}

// sll and srl with format: srl t0 t1 6
int sll_srl(vector<string> &v) {
  int res = map[v[2]]; //get t1
  res = res << 5;
  res += map[v[1]]; //get t0
  res = res << 5;
  res += stoi(v[3]); // get the constant
  res = res << 6; // shift left 6 bits for sll or srl instruction code
  res += map[v[0]];
  return res;
}

//lw and sw with format: lw t0 4(t1)
int lw_sw(vector<string> &v) {
  int res = map[v[0]];
  res = res << 5;
  res += map[v[3]]; // append t1 into res
  res = res << 5;
  res += map[v[1]]; // append t0 into res
  res = res << 16;
  res += stoi(v[2]); // append the constant
  return res;
}
//syscall with format: syscall
int syscall(vector<string> &v) {
   int res = map[v[0]];
   return res;
}

//j with format: j label
int j(vector<string> &v){
   int label_line = map_instr[v[1]]; // get the corresponding line for label and append
   int res = map["j"];
   res = res << 26;
   res += label_line;
   return res;
}

//jal with format: jal label
int jal(vector<string> &v){
   int label_line = map_instr[v[1]];
   int res = map["jal"];
   res = res << 26;
   res += label_line;
   return res;
}

//jr with format: jr $ra
int jr(vector<string> &v){
   int reg = map[v[1]];
   int command = map[v[0]];
   int res = reg;
   res = res << 21;
   res += command;
   return res;
   /*
   int label_line = map_instr[v[1]];
   int res = map["jr"];
   res = res << 26;
   res += label_line;
   return res;
   */
}

// jalr with format: jalr $s0
int jalr(vector<string> &v){
   int res = 0;
   if (v.size() == 2){
       int command = map[v[0]];
       res = 31;
       res = res << 10;
       int reg = map[v[1]];
       res = res + reg;
       res = res << 11;
       res = res + command;
   }
   else{
       int command = map[v[0]];
       res = map[v[2]];
       res = res << 10;
       int reg = map[v[1]];
       res = res + reg;
       res = res << 11;
       res = res + command;
   }
   return res;
}

// beq with format: beq $t0, $t1, $t2
int beq(int cur_line, vector<string> &v){
    int res = map["beq"];
    res = res << 5;
    res += map[v[1]];
    res = res << 5;
    res += map[v[2]];
    res = res << 16;
    int label_line = map_instr[v[3]];
    //int offset = label_line;
    int offset = label_line - cur_line; //cur_line is from i
    if (offset >= 0){
        res += (offset-1);
        return res;
    } //if negative:
    int temp = 1;
    temp = temp << 16;
    temp += offset;
    res += temp;
    res -= 1;
    return res;
}

// bne with format: bne $t0, $t1, $t2
int bne(int cur_line, vector<string> &v){
    int res = map["bne"];
    res = res << 5;
    res += map[v[1]];
    res = res << 5;
    res += map[v[2]];
    res = res << 16;
    int label_line = map_instr[v[3]];
    //int offset = label_line;
    int offset = label_line - cur_line; //cur_line is from i
    if (offset >= 0){
        res += (offset-1);
        return res;
    } //if negative:
    int temp = 1;
    temp = temp << 16;
    temp += (offset-1);
    res += temp;
    return res;
}

int clo_clz(vector<string> &v){
    int res = 28;
    res = res << 5;
    res += map[v[2]];
    res = res << 10;
    res += map[v[1]];
    res = res << 11;
    res += map[v[0]];
    return res;
}


// separate each part of instrustion and put it into vector
void SplitString(string s, vector<string> &v){
  string temp = "";
  int round = 0;
  for(int i=0;i<s.length();++i){
      if(s[i]==' ' || s[i]=='(' || s[i]==','){
          round++;
          if (round == 1) {
              v.push_back(temp);
          }
          temp = "";
      } else{
          if (s[i]!=')') {
              temp.push_back(s[i]);
              round = 0;
          }
      }
  }
  v.push_back(temp);
}

// store all registers and instruction name into hashmap with the its demical value.
unordered_map<string,int> createHashMap(unordered_map<string,int> map){
   map["add"] = 32;
   map["addi"] = 8;
   map["sub"] = 34;
   map["mult"] = 24;
   map["div"] = 26;
   map["mflo"] = 18;
   map["mfhi"] = 16;
   map["sll"] = 0;
   map["srl"] = 2;
   map["lw"] = 35;
   map["sw"] = 43;
   map["slt"] = 42;
   map["beq"] = 4;
   map["bne"] = 5;
   map["j"] = 2;
   map["jal"] = 3;
   map["jr"] = 8;
   map["jalr"] = 9;
   map["syscall"] = 12;
   map["$t0"] = 8;
   map["$t1"] = 9;
   map["$t2"] = 10;
   map["$t3"] = 11;
   map["$t4"] = 12;
   map["$t5"] = 13;
   map["$t6"] = 14;
   map["$t7"] = 15;
   map["$t8"] = 24;
   map["$t9"] = 25;
   map["$s0"] = 16;
   map["$s1"] = 17;
   map["$s2"] = 18;
   map["$s3"] = 19;
   map["$s4"] = 20;
   map["$s5"] = 21;
   map["$s6"] = 22;
   map["$s7"] = 23;
   map["$t8"] = 24;
   map["$t9"] = 25;
   map["$k0"] = 26;
   map["$k1"] = 27;
   map["$gp"] = 28;
   map["$sp"] = 29;
   map["$s8"] = 30;
   map["$ra"] = 31;
   map["$8"] = 8;
   map["$9"] = 9;
   map["$10"] = 10;
   map["$11"] = 11;
   map["$12"] = 12;
   map["$13"] = 13;
   map["$14"] = 14;
   map["$15"] = 15;
   map["$16"] = 16;
   map["$17"] = 17;
   map["$18"] = 18;
   map["$19"] = 19;
   map["$20"] = 20;
   map["$21"] = 21;
   map["$22"] = 22;
   map["$23"] = 23;
   map["$24"] = 24;
   map["$25"] = 25;
   map["$26"] = 26;
   map["$27"] = 27;
   map["$28"] = 28;
   map["$29"] = 29;
   map["$30"] = 30;
   map["$31"] = 31;
   map["$zero"] = 0;
   map["$v0"] = 2;
   map["$v1"] = 3;
   map["$a0"] = 4;
   map["$a1"] = 5;
   map["$a2"] = 6;
   map["$a3"] = 7;
   map["$at"] = 1;
   map["$r0"] = 0;
   map["$1"] = 1;
   map["$2"] = 2;
   map["$3"] = 3;
   map["$4"] = 4;
   map["$5"] = 5;
   map["$6"] = 6;
   map["$7"] = 7;
   map["clo"] = 33;
   map["clz"] = 32;
   return map;
}

// c++ version of trim()
void strip(std::string& str) {
   if (str.length() == 0) {
      return;
   }
   auto start_it = str.begin();
   auto end_it = str.rbegin();
   while (std::isspace(*start_it)) {
      ++start_it;
      if (start_it == str.end()) break;
   }
   while (std::isspace(*end_it)) {
      ++end_it;
      if (end_it == str.rend()) break;
   }
   int start_pos = start_it - str.begin();
   int end_pos = end_it.base() - str.begin();
   str = start_pos <= end_pos ? std::string(start_it, end_it.base()) : "";
}
