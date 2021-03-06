#include <iostream>
#include <string>
#include <sstream>

//Custom Font
#define RED "\x1B[31m"
#define YLW "\x1b[33m"
#define NRM "\x1B[0m"
#define GRN "\x1B[32m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define BOLD "\x1b[1m"
#define clear() printf("\033[H\033[J")

//Advance Declaration
void parser(std::string), end_program(), help(), print_history();

//Preferences
bool DEBUG_LOG = false;
bool TEST_LOG = false;


//Custom Class
class CVector{
    private:
        double x, y, z;
    public:
        CVector(){}; //Empty construstion
        CVector(double, double, double);
        double get_X();
        double get_Y();
        double get_Z();
        void set_value(double, double, double);
        CVector operator + (const CVector&);
        CVector operator - (const CVector&);
        CVector operator * (const CVector&);
};

//Global Variable
CVector current(0, 0, 0);
CVector* result_history = new CVector[20];
CVector* opt_history = new CVector[20];
char* symbol_history = new char[20];
int result_pos = 0;
int opt_pos = 0;
int sym_pos = 0;


CVector::CVector(double a, double b, double c){
    x = a;
    y = b;
    z = c;
}

double CVector::get_X(){
    return x;
}

double CVector::get_Y(){
    return y;
}

double CVector::get_Z(){
    return z;
}

void CVector::set_value(double a, double b, double c){
    x = a;
    y = b;
    z = c;
}

CVector CVector::operator+ (const CVector& param){
    CVector result(0, 0, 0);
    result.x = x + param.x;
    result.y = y + param.y;
    result.z = z + param.z;
    return result;
}

CVector CVector::operator- (const CVector& param){
    CVector result(0, 0, 0);
    result.x = x - param.x;
    result.y = y - param.y;
    result.z = z - param.z;
    return result;
}

CVector CVector::operator* (const CVector& param){
    CVector result(0, 0, 0);
    result.x = y*(param.z) - (param.y)*z;
    result.y = -(x*(param.z) - (param.x)*z);
    result.z = x*(param.y) - (param.x)*y;
    return result;
}

void print_vector(CVector to_print){
    printf("===================================\n");
    printf("HISTORY    CLEAR      HELP     EXIT\n");
    printf("===================================\n\n");
    std::cout<<BOLD"   "
        <<"("<<to_print.get_X()<<", "<<to_print.get_Y()<<", "<<to_print.get_Z()<<")"NRM<<std::endl;
}

void start_rotate(CVector current){
    print_vector(current);
    std::cout<<"=>";
    std::string input;
    getline(std::cin, input);
        if(DEBUG_LOG == true) {std::cout<<"Input in IO is: "<<input<<std::endl;}
        printf("\n");
    parser(input);
}

bool notValid(char c){
    switch(c){
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case ',':
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
            return false;
        default:
            return true;
    }
}

bool notNumber(char c){
    switch(c){
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case ',':
            return false;
        default:
            return true;
    }
}

bool isOperation(char c){
    switch(c){
        case '+':
        case '-':
        case '*':
            return true;
        default:
            return false;
    }
}

bool filter_comma(std::string input){
    int length = input.length();
    int i, c=0;
    bool result = false;
    for(i = 0; i<=length; i++){
        if(input[i]==',') c++;
    }
    if(c==2) result=true;
    if(result==false) {
        clear();
        printf(YLW"INVALID NUMBER OF ELEMENTS\n\n"NRM);
    }
    return result;
}

bool filter_symbol(char symbol){
    bool result = false;
    if(symbol == '+' || symbol == '-' || symbol == '*') {result = true;}
    if(result==false) {
        clear();
        printf(YLW "INVALID OPERATION\n\n" NRM);
    }
    return result;
}

bool filter_order(std::string input){
    int i;
    bool par_flag = false;
    bool num_flag = false;
    bool result = true;
    int length = input.length();
    for(i = 0; i<=length; i++){
        if(input[i]=='(') {par_flag = true;}
        if( (!notNumber(input[i])) && par_flag==false) {result=false;}
    }
    if(result==false) {
        clear();
        printf(YLW "INVALID NUMBER BEFORE PARENTHESIS\n\n" NRM);
    }
    return result;
}

void clear_history(){
    for(int i = 0; i<19; i++){
        symbol_history[i] = '/';
    }
    result_pos = 0;
    opt_pos = 0;
    sym_pos = 0;
    clear();
    printf("HISTORY HAS BEEN CLEARED!\n\n");
    start_rotate(current);
}

void history_overflow(){
    printf("HISTORY MEMORY FULL, USE CLEAR TO RESET\n");
    for(int i=0; i<18; i++){
        result_history[i] = result_history[i+1];
        opt_history[i] = opt_history[i+1];
        symbol_history[i] = symbol_history[i+1];
    }
}

void record_history(CVector vc, char c){
    if(result_pos==18 || opt_pos==18 || sym_pos==18){
        history_overflow();
        result_history[18] = current;
        opt_history[18] = vc;
        symbol_history[18] = c;
    }
    else{
        result_history[result_pos] = current;
        result_pos++;
        opt_history[opt_pos] = vc;
        opt_pos++;
        symbol_history[sym_pos] = c;
        sym_pos++;
    }
}

void plus(CVector vc){
    record_history(vc, '+');

    current = current + vc;
    clear();
    start_rotate(current);
}

void minus(CVector vc){
    record_history(vc, '-');

    current = current - vc;
    clear();
    start_rotate(current);
}

void multi(CVector vc){
    record_history(vc, '*');

    current = current * vc;
    clear();
    start_rotate(current);
}

void parser(std::string input){
    char symbol;
    std::string opt;
    double* numbers = new double[3];
    int pos = 0;
    if(input == "help" || input == "HELP") help();
    if(input == "exit" || input == "EXIT") end_program();
    if(input == "history" || input == "HISTORY") print_history();
    if(input == "clear" || input == "CLEAR") clear_history();
        if(DEBUG_LOG == true) {std::cout<<"Input before remove is: "<<input<<std::endl;}
    input.erase(std::remove_if(input.begin(), input.end(), isspace), input.end());
        if(DEBUG_LOG == true) {std::cout<<"Input after 1st remove is: "<<input<<std::endl;}
    input.erase(std::remove_if(input.begin(), input.end(), &notValid), input.end());
        if(DEBUG_LOG == true) {std::cout<<"Input after 2nd remove is: "<<input<<std::endl;}
    if(input == ""){
        printf(RED"INVALID EXPRESSION\n\n"NRM);
        start_rotate(current);
    }
    symbol = input.at(0);
        if(DEBUG_LOG == true) {std::cout<<"Parsed symbol is: "<<symbol<<std::endl;}
    //Final syntax check before sending to process
    bool comma_check = filter_comma(input);
    bool syntax_check = filter_symbol(symbol);
    bool order_check = filter_order(input);
    if(!(comma_check && syntax_check && order_check)){
        printf(RED"INVALID EXPRESSION\n\n"NRM);
        start_rotate(current);
    }//Input verified and to be processed
    input.erase(std::remove_if(input.begin(), input.end(), &notNumber), input.end());
        if(DEBUG_LOG == true) {std::cout<<"Input after 3rd remove is: "<<input<<std::endl;}
    //Parsing individual numbers
    std::istringstream ss(input);
    std::string token;
    while(std::getline(ss, token, ',')) {
            if(DEBUG_LOG == true) {std::cout << token << '\n';}
        double current_number = 0;
        std::stringstream(token) >> current_number;
        numbers[pos] = current_number;
        pos++;
    }//Parsing complete, running calculations
        if(DEBUG_LOG == true) {std::cout<<numbers[0]<<"/"<<numbers[1]<<"/"<<numbers[2]<<std::endl;}
    CVector pending(numbers[0], numbers[1], numbers[2]);
    if(symbol=='+') plus(pending);
    if(symbol=='-') minus(pending);
    if(symbol=='*') multi(pending);
}

void print_history(){
    clear();
    printf("-----------HISTORY-----------\n\n");
    for(int i=0; i<19 && isOperation(symbol_history[i]); i++){
        std::cout << "=> (" << result_history[i].get_X() << ", " << result_history[i].get_Y() << ", " << result_history[i].get_Z() << ") "
                  << symbol_history[i] 
                  << " (" << opt_history[i].get_X() << ", " << opt_history[i].get_Y() << ", " << opt_history[i].get_Z() << ") \n";
    }
    printf("\n");
    start_rotate(current);
}

//Tests
void test_plus(){
    printf(GRN "Testing plus operation...\n" NRM);
    CVector vector_A(0,0,0);
    CVector vector_B(1,2,3);
    CVector vector_C(4,5,6);
    vector_A = vector_B + vector_C;
    std::cout <<"Testing plus function... "<< vector_A.get_X() << "," << vector_A.get_Y() << "," << vector_A.get_Z() << std::endl;
    printf("Expecting... 5,7,9\n");
}

void test_minus(){
    printf(GRN "Testing minus operation...\n" NRM);
    CVector vector_A(0,0,0);
    CVector vector_B(1,2,3);
    CVector vector_C(4,5,6);
    vector_A = vector_B - vector_C;
    std::cout <<"Testing minus function... "<< vector_A.get_X() << "," << vector_A.get_Y() << "," << vector_A.get_Z() << std::endl;
    printf("Expecting... -3,-3,-3\n");
}

void test_multi(){
    printf(GRN"Testing multiplication operation\n"NRM);
    CVector vector_A(0,0,0);
    CVector vector_B(1,2,3);
    CVector vector_C(4,5,6);
    vector_A = vector_B * vector_C;
    std::cout << "Testing multiplication function... " << vector_A.get_X() << "," << vector_A.get_Y() << "," << vector_A.get_Z() << std::endl;
    printf("Expecting... -3, 6, -3\n");
}

void test(){
    printf(GRN "Initialising tests...\n" NRM);
    test_plus();
    test_minus();
    test_multi();
    printf(BLU "Testing complete\n" NRM);
}

void help(){
    clear();
    printf(YLW"================HELP===============\n");
    printf("1.SYNTAX: OPERATION + VECTOR\n");
    printf("2.EXAMPLE *(1,2,3)\n");
    printf("3.SUPPORTED OPERATION: +, -, *\n");
    printf("4.ENTER HISTORY TO VIEW PAST 15 ENTRIES\n");
    printf("5.ENTER EXIT TO ESCAPE\n\n"NRM);
    start_rotate(current);
}

void end_program(){
    clear();
    printf(RED "-----------EXIT-----------\n\n" NRM);
    exit(0);
}

int main(){
    if(TEST_LOG==true) test();

    printf("\n\n");
    printf("    / ======= \\    \n");
    printf("   / __________\\  \n");
    printf("  | ___________ |  \n");
    printf("  | | -       | |  \n");
    printf("  | | v0.0.6  | |  \n");
    printf("  | |_________| |  \n");
    printf("  \\=___________   \n");
    printf("  / =========== \\ \n");
    printf(" / ::::::::::::: \\ \n");
    printf("(_________________) \n\n");
    printf(YLW"For help on syntax, enter: help\n"NRM);
    printf("\n\n");

    current.set_value(0,0,0);
    start_rotate(current);

    return 0;
}
