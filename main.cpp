#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
using namespace std;

static int LineLength = 32;
static string AllLr (LineLength, 'L');
static string AllL = AllLr + "\n";
static string defLr (LineLength-3, 'L');
static string defL = defLr + "\n";
static string empty = "LLL" + defLr + "\nHLL" + defLr + "\n";

string HL(int);
string HL(int bit) {return bit?"H":"L";};

void Line(string&, int);
void Line(string& str, int bit) {str = "LL" + HL(bit) + defL + "HL" + HL(bit) + defL;}

void Alln(int*, int);
void Alln(int* arr, int n, int num) {for (int i=0;i<n;i++) {arr[i]=num;}}

template <typename T>
void input(T &t) {
    char c;
    cin.get(c);
    if (c!= '\n') {
        cin.putback(c);
        cin >> t;
        cin.get();
    }
}

bool check();
bool check() {
    string s;
    cin >> s;
    if ((s!="n")&(s!="N")&(s!="no")&(s!="No")) {
        return true;
    } else {
        return false;
    }
}

double BSLN(int*, int);
double BSLN(int* vref, int st) {
    double v = 0;
    v -= vref[st]*4.6875;
    v -= vref[st+1]*9.375;
    v -= vref[st+2]*18.75;
    v -= vref[st+3]*37.5;
    v -= vref[st+4]*75;
    v -= vref[st+5]*150;
    v -= vref[st+6]*300;
    v += vref[st+7]*600;
    return v;
}

void Vrefbit(int*, int, double);
void Vrefbit(int* arr, int s, double vref) {
    arr[s+7] = vref>0?1:0;
    vref = vref>0?vref-600:vref;
    arr[s+6] = vref>-300?0:1;
    vref = vref>-300?vref:vref+300;
    arr[s+5] = vref>-150?0:1;
    vref = vref>-150?vref:vref+150;
    arr[s+4] = vref>-75?0:1;
    vref = vref>-75?vref:vref+75;
    arr[s+3] = vref>-37.5?0:1;
    vref = vref>-37.5?vref:vref+37.5;
    arr[s+2] = vref>-18.75?0:1;
    vref = vref>-18.75?vref:vref+18.75;
    arr[s+1] = vref>-9.375?0:1;
    vref = vref>-9.375?vref:vref+9.375;
    arr[s] = vref>-4.6875?0:1;
}

int main() {
//    system("CHCP 65001>nul");
    int input_dig;
    int bitTotal = 184;
    //MM_SPI_ASD_104B(1)
    int bitGATE_DELAY = 8, GATE_DELAY[8];
    Alln(GATE_DELAY, 8, 1);
    int bitASD_SPI_REG = 96, ASD_SPI_REG[96] = {1};
    Alln(ASD_SPI_REG, 96, 1);
    //MM_SPI_ASD_8B(1)
    int bitTEST_IN = 8, TEST_IN[8];
    Alln(TEST_IN, 8, 1);
    //MM_SPI_ASD_8B(2)
    int ENB = 1, RST = 0, NC0 = 0, OSC_EN = 1, TEST_EN = 1, POL = 1, R = 0, TP = 1;
    //MM_SPI_ASD_64B(1)
    int bitVref = 64, Vref[64] = {0};
    for (int i=0;i<8;i++) {
        Vref[8*i] = 0;      Vref[8*i+1] = 0;    Vref[8*i+2] = 0;    Vref[8*i+3] = 0;
        Vref[8*i+4] = 1;    Vref[8*i+5] = 1;    Vref[8*i+6] = 1;    Vref[8*i+7] = 1;
    }

    FILE* OutPutFile;
    FILE* ConfigFile;
    FILE* ConfigSet;
    string OutPutName;
    string cacheName;
    int Config_Len;

    cacheName = "Script_config";
    cout << "Filename:\n";
    cin >> OutPutName;
    ConfigFile = fopen(cacheName.c_str(),"a+");
    fseek(ConfigFile,0,SEEK_END);
    Config_Len = ftell(ConfigFile);
    if (Config_Len<184) {
        cout << "Wrong configuration file. Use default configurations.\n";
        cout << "Default set-up for 184-bit SPI register:\n" <<
             "MM_SPI_ASD_104B |  GATE_DELAY<8>  |------------------ ASD_SPI_REG<96> ------------------|\n" <<
             "                |    HHHH HHHH    |       22 bits for Type-A & 20 bits for Type-B       |\n" <<
             "                                  |       EN_EXTBIAS:H      |         EN_MON:H          |\n" <<
             "---------------------- Type-A SPI |              HHHHH HHHHH HHHHH HHHHH                |\n" <<
             "---------------------- Type-B SPI |              HHHHH HHHHH HHHHH HHHHH                |\n" <<
             "MM_SPI_ASD_8B   |    TEST_IN<8>   |\n" <<
             "                |    HHHH HHHH    |\n" <<
             "MM_SPI_ASD_8B   |   ENB  |   RST  |   NC0  | OSC_EN | TEST_EN|   POL  |    R   |   TP   |\n" <<
             "                |    H   |    L   |    L   |    H   |    H   |    H   |    L   |    H   |\n" <<
             "MM_SPI_ASD_64B  |------------------------------- Vref<64> ------------------------------|\n" <<
             "                |LLLLHHHH|LLLLHHHH|LLLLHHHH|LLLLHHHH|LLLLHHHH|LLLLHHHH|LLLLHHHH|LLLLHHHH|\n" <<
             "                | +75 mV | +75 mV | +75 mV | +75 mV | +75 mV | +75 mV | +75 mV | +75 mV |\n";
    }
    else {
        fseek(ConfigFile,0,SEEK_SET);
        for (int i=0;i<bitGATE_DELAY;i++) {GATE_DELAY[i]= fgetc(ConfigFile);}
        for (int i=0;i<bitASD_SPI_REG;i++) {ASD_SPI_REG[i]= fgetc(ConfigFile);}
        for (int i=0;i<bitTEST_IN;i++) {TEST_IN[i]= fgetc(ConfigFile);}
        ENB = fgetc(ConfigFile);
        RST = fgetc(ConfigFile);
        NC0 = fgetc(ConfigFile);
        OSC_EN = fgetc(ConfigFile);
        TEST_EN = fgetc(ConfigFile);
        POL = fgetc(ConfigFile);
        R = fgetc(ConfigFile);
        TP = fgetc(ConfigFile);
        for (int i=0;i<bitVref;i++) {Vref[i]= fgetc(ConfigFile);}
    }
    fclose(ConfigFile);

    OutPutFile = fopen(OutPutName.c_str(),"w+");
    if (!OutPutFile) {
        std::cerr << "Unable to open file!" << std::endl;
        return 1;
    }

    cout << "Current set-up for 184-bit SPI register:\n" <<
         "MM_SPI_ASD_104B |  GATE_DELAY<8>  |------------------ ASD_SPI_REG<96> ------------------|\n" <<
         "                |    ";
    for (int i=0;i<bitGATE_DELAY;i++) {cout << HL(GATE_DELAY[i]);   if (i==3) {cout<<" ";}}
    cout << "    |       20 bits for Type-A & 20 bits for Type-B       |\n" <<
         "                                  |       EN_EXTBIAS:"<<HL(ASD_SPI_REG[0]) <<
         "      |         EN_MON:"<<HL(ASD_SPI_REG[1]) << "          |\n---------------------- Type-A SPI |              ";
    for (int i=2;i<bitASD_SPI_REG;i++) {
        if (i<42) {cout << HL(ASD_SPI_REG[i]);}
        if (i==0||i==1||i==6||i==11||i==16||i==26||i==31||i==36) {cout<<" ";}
        if (i==21) {cout<<"                |\n---------------------- Type-B SPI |              ";}
        if (i==41) {cout<<"                |\nMM_SPI_ASD_8B   |    TEST_IN<8>   |\n                |    ";}
    }
    for (int i=0;i<bitTEST_IN;i++) {cout << HL(TEST_IN[i]); if (i==3) {cout<<" ";}}
    cout << "    |\nMM_SPI_ASD_8B   |   ENB  |   RST  |   NC0  | OSC_EN | TEST_EN|   POL  |    R   |   TP   |\n                |    " ;
    cout << HL(ENB) << "   |    " << HL(RST)<<"   |    " << HL(NC0)<<"   |    " << HL(OSC_EN)
         << "   |    " << HL(TEST_EN) << "   |    " << HL(POL) << "   |    " << HL(R) << "   |    "
         << HL(TP)<<"   |\n"
         << "MM_SPI_ASD_64B  |------------------------------- Vref<64> ------------------------------|\n                |";

    for (int i1=0;i1<8;i1++) {for (int i2=0;i2<8;i2++) {cout << HL(Vref[i1*8+i2]);} cout<<"|";}
    cout << "\n";
    for (int i=0;i<8;i++) {cout<<"Ch."<<i<<": "<<BSLN(Vref,i*8)<<" mV\n";}
    cout << "\nProceed without change?(y/n)(All options yes by default.)(H-1/L-0)\n";

    bool flagAll = !check();
    bool flagGDREG = false;
    bool flagGD = false, flagGD1 = false, flagGD2 = false;
    int flagGDn = 0;
    bool flagREG = false, flagREG0 = false, flagREGA = false, flagREGA1 = false, flagREGB = false,
                                                              flagREGB1 = false, flagREGAone = false,
                                                              flagREGBone = false, flagREGAalloff = false, flagREGBalloff = false;
    int flagREGAno = 0, flagREGBno = 0;
    bool flagTI = false, flagTI1 = false;
    bool flag8B = false;
    bool flagVref = false, flagVref1 = false, flagVrefone = false, flagVrefV = false;
    int flagVrefno = 0;
    double input_vref;
//#define CheckFlagGDREG
#ifdef CheckFlagGDREG
if (flagAll) {cout << "Change GATE_DELAY<8> & ASD_SPI_REG<96>?(y/n)\n";flagGDREG = check();}
#endif
#ifndef CheckFlagGDREG
    flagGDREG = true;
#endif
    if (flagGDREG) {cout << "-> Change GATE_DELAY?(y/n)\n";flagGD = check();}
    if (flagGD) {cout << "--> All the same?(y/n)\n";flagGD1 = check();}
    if (flagGD&&!flagGD1) {cout << "--> Set the first n bits?(y/n)\n";flagGD2 = check();}
    if (flagGD&&flagGD2) {
        cout << "---> n:\n";
        cin >> flagGDn;
        while (!(flagGDn>=0 && flagGDn<=8)) {cout << "Wrong output.\n"; cin >> flagGDn;}
        for (int i = 0; i<flagGDn; i++) {GATE_DELAY[i] = 1;}
    }
    if (flagGD&&flagGD1&&!flagGD2) {
        cout << "---> H/L(1/0):\n"; cin >> input_dig;
        while (!(input_dig == 0 || input_dig == 1)) {cout << "Wrong output.\n"; cin >> input_dig;}
        for (int tem = 0; tem < bitGATE_DELAY; tem++) {GATE_DELAY[tem] = input_dig;}
    }
    if (flagGD&&!flagGD1&&!flagGD2) {
        for (int tem = 0; tem < bitGATE_DELAY; tem++) {
            cout << "GATE_DELAY<" << tem << ">:\n";
            cin >> GATE_DELAY[tem];
            while (!(GATE_DELAY[tem] == 0 || GATE_DELAY[tem] == 1)) {cout << "Wrong output.\n"; cin >> GATE_DELAY[tem];}
        }
    }
    if (flagGDREG) {cout << "-> Change ASD_SPI_REG?(y/n)\n";flagREG = check();}
    if (flagREG) {cout << "--> Change EN_EXTBIAS and EN_MON?(y/n)\n";flagREG0 = check();}
    if (flagREG0) {
        cout << "REG<0>(EN_EXTBIAS):\n";    cin >> ASD_SPI_REG[0];
        while (!(ASD_SPI_REG[0] == 0 || ASD_SPI_REG[0] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[0];}
        cout << "REG<1>(EN_MON):\n";        cin >> ASD_SPI_REG[1];
        while (!(ASD_SPI_REG[1] == 0 || ASD_SPI_REG[1] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[1];}
    }
    if (flagREG) {cout << "--> Turn off all sets for Type A?(y/n)\n";flagREGAalloff = check();}
    if (flagREGAalloff) {for(int i=2;i<22;i++) {ASD_SPI_REG[i]=0;}}
    if (flagREG)  {cout << "--> Change REG for Type A?(y/n)\n";flagREGA = check();}
    if (flagREGA/*&&!flagREGA1*/) {cout << "---> Set only one channel?(y/n)\n";flagREGAone = check();}
    if (flagREGA&&!flagREGAone) {cout << "---> All four sets the same?(y/n)\n";flagREGA1 = check();}
    if (flagREGAone) {
        cout << "----> Set which channel?(1/2/3/4)\n";
        cin>>flagREGAno;
        while (flagREGAno < 1 || flagREGAno > 4) {cout << "Wrong output."; cin >> flagREGAno;}
        cout << "REG(MON output control, 1:OUT_CRRC, 0:OUT_SF):\n";    cin >> ASD_SPI_REG[flagREGAno*5-3];
        while (!(ASD_SPI_REG[flagREGAno*5-3] == 0 || ASD_SPI_REG[flagREGAno*5-3] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5-3];}
        cout << "REG(TPENB):\n";    cin >> ASD_SPI_REG[flagREGAno*5-2];
        while (!(ASD_SPI_REG[flagREGAno*5-2] == 0 || ASD_SPI_REG[flagREGAno*5-2] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5-2];}
        cout << "REG(EN_MON):\n";    cin >> ASD_SPI_REG[flagREGAno*5-1];
        while (!(ASD_SPI_REG[flagREGAno*5-1] == 0 || ASD_SPI_REG[flagREGAno*5-1] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5-1];}
        cout << "REG(EN_COMP):\n";    cin >> ASD_SPI_REG[flagREGAno*5];
        while (!(ASD_SPI_REG[flagREGAno*5] == 0 || ASD_SPI_REG[flagREGAno*5] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5];}
        cout << "REG(EN_MON_VTH):\n";    cin >> ASD_SPI_REG[flagREGAno*5+1];
        while (!(ASD_SPI_REG[flagREGAno*5+1] == 0 || ASD_SPI_REG[flagREGAno*5+1] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5+1];}
    }
    if (flagREGA&&flagREGA1&&!flagREGAone) {
        cout << "REG(MON output control, 1:OUT_CRRC, 0:OUT_SF):\n";    cin >> ASD_SPI_REG[2];
        while (!(ASD_SPI_REG[2] == 0 || ASD_SPI_REG[2] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[2];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+2] = ASD_SPI_REG[2];}
        cout << "REG(TPENB):\n";    cin >> ASD_SPI_REG[3];
        while (!(ASD_SPI_REG[3] == 0 || ASD_SPI_REG[3] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[3];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+3] = ASD_SPI_REG[3];}
        cout << "REG(EN_MON):\n";    cin >> ASD_SPI_REG[4];
        while (!(ASD_SPI_REG[4] == 0 || ASD_SPI_REG[4] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[4];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+4] = ASD_SPI_REG[4];}
        cout << "REG(EN_COMP):\n";    cin >> ASD_SPI_REG[5];
        while (!(ASD_SPI_REG[5] == 0 || ASD_SPI_REG[5] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[5];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+5] = ASD_SPI_REG[5];}
        cout << "REG(EN_MON_VTH):\n";    cin >> ASD_SPI_REG[6];
        while (!(ASD_SPI_REG[6] == 0 || ASD_SPI_REG[6] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[6];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+6] = ASD_SPI_REG[6];}
    }
    if (flagREGA&&!flagREGA1&&!flagREGAone) {
        cout << "MON(1:CRRC,0:SF), TPENB, EN_MON, EN_COMP, EN_MON_VTH:\n";
        for (int tem = 2; tem < 22; tem++) {
            cout << "REG<" << tem << ">:\n";    cin >> ASD_SPI_REG[tem];
            while (!(ASD_SPI_REG[tem] == 0 || ASD_SPI_REG[tem] == 1)) {cout << "Wrong output.\n";cin >> ASD_SPI_REG[tem];}
        }
    }
    if (flagREG) {cout << "--> Turn off all sets for Type B?(y/n)\n";flagREGBalloff = check();}
    if (flagREGBalloff) {for(int i=22;i<42;i++) {ASD_SPI_REG[i]=0;}}
    if (flagREG)  {cout << "--> Change REG for Type B?(y/n)\n";flagREGB = check();}
    if (flagREGB/*&&!flagREGB1*/) {cout << "---> Set only one channel?(y/n)\n";flagREGBone = check();}
    if (flagREGB&&!flagREGBone) {cout << "---> All four sets the same?(y/n)\n";flagREGB1 = check();}
    if (flagREGBone) {
        cout << "----> Set which channel?(1/2/3/4)\n";
        cin>>flagREGAno;
        while (flagREGAno < 1 || flagREGAno > 4) {cout << "Wrong output.\n"; cin >> flagREGAno;}
        cout << "REG(MON output control, 1:OUT_CRRC, 0:OUT_SF):\n";    cin >> ASD_SPI_REG[flagREGAno*5-3];
        while (!(ASD_SPI_REG[flagREGAno*5+17] == 0 || ASD_SPI_REG[flagREGAno*5+17] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5+17];}
        cout << "REG(TPENB):\n";    cin >> ASD_SPI_REG[flagREGAno*5-2];
        while (!(ASD_SPI_REG[flagREGAno*5+18] == 0 || ASD_SPI_REG[flagREGAno*5+18] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5+18];}
        cout << "REG(EN_MON):\n";    cin >> ASD_SPI_REG[flagREGAno*5-1];
        while (!(ASD_SPI_REG[flagREGAno*5+19] == 0 || ASD_SPI_REG[flagREGAno*5+19] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5+19];}
        cout << "REG(EN_COMP):\n";    cin >> ASD_SPI_REG[flagREGAno*5];
        while (!(ASD_SPI_REG[flagREGAno*5+20] == 0 || ASD_SPI_REG[flagREGAno*5+20] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5+20];}
        cout << "REG(EN_MON_VTH):\n";    cin >> ASD_SPI_REG[flagREGAno*5+1];
        while (!(ASD_SPI_REG[flagREGAno*5+21] == 0 || ASD_SPI_REG[flagREGAno*5+21] == 1)) {cout << "Wrong output.\n";    cin >> ASD_SPI_REG[flagREGAno*5+21];}
    }
    if (flagREGB&&flagREGB1&&!flagREGBone) {
        cout << "REG(MON output control, 1:OUT_CRRC, 0:OUT_SF):\n";    cin >> ASD_SPI_REG[22];
        while (!(ASD_SPI_REG[22] == 0 || ASD_SPI_REG[22] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[22];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+22] = ASD_SPI_REG[22];}
        cout << "REG(TPENB):\n";    cin >> ASD_SPI_REG[23];
        while (!(ASD_SPI_REG[23] == 0 || ASD_SPI_REG[23] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[23];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+23] = ASD_SPI_REG[23];}
        cout << "REG(EN_MON):\n";    cin >> ASD_SPI_REG[24];
        while (!(ASD_SPI_REG[24] == 0 || ASD_SPI_REG[24] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[24];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+24] = ASD_SPI_REG[24];}
        cout << "REG(EN_COMP):\n";    cin >> ASD_SPI_REG[25];
        while (!(ASD_SPI_REG[25] == 0 || ASD_SPI_REG[25] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[25];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+25] = ASD_SPI_REG[25];}
        cout << "REG(EN_MON_VTH):\n";    cin >> ASD_SPI_REG[26];
        while (!(ASD_SPI_REG[26] == 0 || ASD_SPI_REG[26] == 1)) {cout << "Wrong output.";    cin >> ASD_SPI_REG[26];}
        for (int i = 1; i < 4; i++) {ASD_SPI_REG[i*5+26] = ASD_SPI_REG[26];}
    }
    if (flagREGB&&!flagREGB1&&!flagREGBone) {
        cout << "MON(1:CRRC,0:SF), TPENB, EN_MON, EN_COMP, EN_MON_VTH:\n";
        for (int tem = 22; tem < 42; tem++) {
            cout << "REG<" << tem << ">:\n";    cin >> ASD_SPI_REG[tem];
            while (!(ASD_SPI_REG[tem] == 0 || ASD_SPI_REG[tem] == 1)) {cout << "Wrong output.\n"; cin >> ASD_SPI_REG[tem];}
        }
    }
    if (flagAll) {cout << "-> Change TEST_IN<8>?(y/n)\n";flagTI = check();}
    if (flagTI) {cout << "--> All the same?(y/n)\n";flagTI1 = check();}
    if (flagTI&&flagTI1) {
        cin >> input_dig;
        while (!(input_dig == 0 || input_dig == 1)) {cout << "Wrong output.\n";   cin >> input_dig;}
        for (int tem = 0; tem < bitTEST_IN; tem++) {GATE_DELAY[tem] = input_dig; }
    }
    if (flagTI&&!flagTI1) {
        for (int tem = 0; tem < bitTEST_IN; tem++) {
            cout << "TEST_IN<" << tem << ">:\n";    cin >> TEST_IN[tem];
            while (!(TEST_IN[tem] == 0 || TEST_IN[tem] == 1)) {cout << "Wrong output.\n"; cin >> TEST_IN[tem];}
        }
    }
    if (flagAll) {cout << "-> Change the second SPI_8B?(y/n)\n";flag8B = check();}
    if (flag8B) {
        cout << "--> Change ENB?(y/n)\n";     if (check()) {cin >> ENB;     while (!(ENB == 0 || ENB == 1)) {cout << "Wrong output.\n"; cin >> ENB;}}
        cout << "--> Change RST?(y/n)\n";     if (check()) {cin >> RST;     while (!(RST == 0 || RST == 1)) {cout << "Wrong output.\n"; cin >> RST;}}
        cout << "--> Change NC0?(y/n)\n";     if (check()) {cin >> NC0;     while (!(NC0 == 0 || NC0 == 1)) {cout << "Wrong output.\n"; cin >> NC0;}}
        cout << "--> Change OSC_EN?(y/n)\n";  if (check()) {cin >> OSC_EN;  while (!(OSC_EN == 0 || OSC_EN == 1)) {cout << "Wrong output.\n"; cin >> OSC_EN;}}
        cout << "--> Change TEST_EN?(y/n)\n"; if (check()) {cin >> TEST_EN; while (!(TEST_EN == 0 || TEST_EN == 1)) {cout << "Wrong output.\n"; cin >> TEST_EN;}}
        cout << "--> Change POL?(y/n)\n";     if (check()) {cin >> POL;     while (!(POL == 0 || POL == 1)) {cout << "Wrong output.\n"; cin >> POL;}}
        cout << "--> Change R?(y/n)\n";       if (check()) {cin >> R;       while (!(R == 0 || R == 1)) {cout << "Wrong output.\n"; cin >> R;}}
        cout << "--> Change TP?(y/n)\n";      if (check()) {cin >> TP;      while (!(TP == 0 || TP == 1)) {cout << "Wrong output.\n"; cin >> TP;}}
    }
    if (flagAll) {cout << "-> Change Vref<64>?(y/n)\n";flagVref = check();}
    if (flagVref) {cout << "--> Set Vref or set bits?(Vref:y/bits:n)\n";flagVrefV = check();}
    if (flagVref) {cout << "--> Change only one setup?(y/n)\n";flagVrefone = check();}
    if (flagVref&&!flagVrefone) {cout << "--> All setup the same?(y/n)\n";flagVref1 = check();}
    if (flagVref&&flagVrefone&&flagVrefV) {
        cout << "---> Which one?(1~8)\n";
        cin>>flagVrefno;
        flagVrefno--;
        while (flagVrefno < 0 || flagVrefno > 7) {cout << "Wrong output.\n"; cin >> flagVrefno;}
        cin >> input_vref;
        while (input_vref<-600||input_vref>600) {cout << "Wrong output.\n";cin >> input_vref;}
        Vrefbit(Vref,flagVrefno*8,input_vref);
    }
    if (flagVref&&flagVrefone&&!flagVrefV) {
        cout << "---> Which one?(1~8)\n";
        cin>>flagVrefno;
        flagVrefno--;
        while (flagVrefno < 0 || flagVrefno > 7) {cout << "Wrong output.\n"; cin >> flagVrefno;}
        for (int tem = 0; tem < 8; tem++) {
            cout << "Vref<" << tem << ">:\n"; cin >> Vref[flagVrefno*8+tem];
            while (!(Vref[flagVrefno*8+tem] == 0 || Vref[flagVrefno*8+tem] == 1)) {cout << "Wrong output.\n";cin >> Vref[flagVrefno*8+tem];}
        }
    }
    if (flagVref&&!flagVrefone&&flagVref1&&!flagVrefV) {
        for (int tem = 0; tem < 8; tem++) {
            cout << "Vref<" << tem << ">:\n";   cin >> Vref[tem];
            while (!(Vref[tem] == 0 || Vref[tem] == 1)) {cout << "Wrong output.\n";cin >> Vref[tem];}
            for (int tem2 = 1; tem2 < 8; tem2++) {Vref[tem+tem2*8] = Vref[tem];}
        }
    }
    if (flagVref&&!flagVrefone&&!flagVref1&&!flagVrefV) {
        for (int tem = 0; tem < bitVref; tem++) {
            cout << "Vref<" << (tem-tem%8)/8 << "," << tem%8 << ">:\n"; cin >> Vref[tem];
            while (!(Vref[tem] == 0 || Vref[tem] == 1)) {cout << "Wrong output.\n";cin >> Vref[tem];}
        }
    }
    if (flagVref&&flagVref1&&flagVrefV&&!flagVrefone) {
        cin >> input_vref;
        while (input_vref<-600||input_vref>600) {cout << "Wrong output.\n";       cin >> input_vref;}
        for (int tem = 0; tem < 8; tem++) {Vrefbit(Vref,tem*8,input_vref);}
    }
    if (flagVref&&!flagVref1&&flagVrefV&&!flagVrefone) {
        for (int tem = 0; tem < 8; tem++) {
            cout << "Vref-" << tem << ":\n";    cin >> input_vref;
            while (input_vref<-600||input_vref>600) {cout << "Wrong output.\n";   cin >> input_vref;}
            Vrefbit(Vref,tem*8,input_vref);
        }
    }

    //Begin file creation.
    ConfigSet = fopen(cacheName.c_str(),"w+");
    for (int i=0;i<bitGATE_DELAY;i++) {fputc(GATE_DELAY[i],ConfigSet);}
    for (int i=0;i<bitASD_SPI_REG;i++) {fputc(ASD_SPI_REG[i],ConfigSet);}
    for (int i=0;i<bitTEST_IN;i++) {fputc(TEST_IN[i],ConfigSet);}
    fputc(ENB,ConfigSet);
    fputc(RST,ConfigSet);
    fputc(NC0,ConfigSet);
    fputc(OSC_EN,ConfigSet);
    fputc(TEST_EN,ConfigSet);
    fputc(POL,ConfigSet);
    fputc(R,ConfigSet);
    fputc(TP,ConfigSet);
    for (int i=0;i<bitVref;i++) {fputc(Vref[i],ConfigSet);}
    fclose(ConfigFile);

    cout << "Current set-up for 184-bit SPI register:\n" <<
            "MM_SPI_ASD_104B |  GATE_DELAY<8>  |------------------ ASD_SPI_REG<96> ------------------|\n" <<
            "                |    ";
    for (int i=0;i<bitGATE_DELAY;i++) {cout << HL(GATE_DELAY[i]);   if (i==3) {cout<<" ";}}
    cout << "    |       20 bits for Type-A & 20 bits for Type-B       |\n" <<
         "                                  |       EN_EXTBIAS:"<<HL(ASD_SPI_REG[0]) <<
         "      |         EN_MON:"<<HL(ASD_SPI_REG[1]) << "          |\n---------------------- Type-A SPI |              ";
    for (int i=2;i<bitASD_SPI_REG;i++) {
        if (i<42) {cout << HL(ASD_SPI_REG[i]);}
        if (i==0||i==1||i==6||i==11||i==16||i==26||i==31||i==36) {cout<<" ";}
        if (i==21) {cout<<"                |\n---------------------- Type-B SPI |              ";}
        if (i==41) {cout<<"                |\nMM_SPI_ASD_8B   |    TEST_IN<8>   |\n                |    ";}
    }
    for (int i=0;i<bitTEST_IN;i++) {cout << HL(TEST_IN[i]); if (i==3) {cout<<" ";}}
    cout << "    |\nMM_SPI_ASD_8B   |   ENB  |   RST  |   NC0  | OSC_EN | TEST_EN|   POL  |    R   |   TP   |\n                |    "
         << HL(ENB)     <<"   |    " << HL(RST) << "   |    " << HL(NC0) << "   |    " << HL(OSC_EN) << "   |    "
         << HL(TEST_EN) <<"   |    " << HL(POL) << "   |    " << HL(R)   << "   |    " << HL(TP)     << "   |\n"
         << "MM_SPI_ASD_64B  |------------------------------- Vref<64> ------------------------------|\n                |";
    for (int i1=0;i1<8;i1++) {for (int i2=0;i2<8;i2++) {cout << HL(Vref[i1*8+i2]);} cout<<"|";}
    cout << "\n";
    for (int i=0;i<8;i++) {cout<<"Ch."<<i<<": "<<BSLN(Vref,i*8)<<" mV\n";}

    OutPutFile = fopen(OutPutName.c_str(),"w+");
    if (!OutPutFile) {
        std::cerr << "Unable to open file!" << std::endl;
        return 1;
    }
    string CurrLine;
    for (int i1=7;i1>=0;i1--) {for (int i2=7;i2>=0;i2--) {Line(CurrLine, Vref[i1*8+i2]);  fputs(CurrLine.c_str(), OutPutFile);}}
    Line(CurrLine, ENB);    fputs(CurrLine.c_str(), OutPutFile);
    Line(CurrLine, RST);    fputs(CurrLine.c_str(), OutPutFile);
    Line(CurrLine, NC0);    fputs(CurrLine.c_str(), OutPutFile);
    Line(CurrLine, OSC_EN); fputs(CurrLine.c_str(), OutPutFile);
    Line(CurrLine, TEST_EN);fputs(CurrLine.c_str(), OutPutFile);
    Line(CurrLine, POL);    fputs(CurrLine.c_str(), OutPutFile);
    Line(CurrLine, R);      fputs(CurrLine.c_str(), OutPutFile);
    Line(CurrLine, TP);     fputs(CurrLine.c_str(), OutPutFile);
    for (int i=bitTEST_IN-1;i>=0;i--) {Line(CurrLine, TEST_IN[i]);      fputs(CurrLine.c_str(), OutPutFile);}
    for (int i=bitGATE_DELAY-1;i>=0;i--) {Line(CurrLine, GATE_DELAY[i]);   fputs(CurrLine.c_str(), OutPutFile);}
    for (int i=bitASD_SPI_REG-1;i>=0;i--) {Line(CurrLine, ASD_SPI_REG[i]);  fputs(CurrLine.c_str(), OutPutFile);}
    fputs(AllLr.c_str(), OutPutFile);
    fclose(OutPutFile);
    cout<<"File finished.\n";

    return 0;
}