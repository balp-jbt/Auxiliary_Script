#ifndef _DEBUG_CONFIG_
#define _DEBUG_CONFIG_

extern int debug_content_cnt;
extern int debug_title_cnt;
extern int debug_fuction_call_order;

#define TITLE_LENGTH 90
#define TITLE_BLANK_LINE 3
#define DEBUG_FILE_PATH "./debug.txt"
#define FUNC_CALL_LINE  "  |  "
#define SHOW_IDX false

#define MIN(A, B) (((A) < (B)) ? (A) : (B))
#define MAX(A, B) (((A) < (B)) ? (B) : (A))

#define DEBUG_OPEN_FILE_INSTR ofstream outf = alert_assistant()
#define DEBUG_CLOSE_FILE_INSTR outf.close()

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

inline ofstream alert_assistant() {
    ofstream outf;
    outf.open(DEBUG_FILE_PATH, ios::app);
    if (! outf.good()) {
        for (int iii = 0; iii < 200; iii++) 
            cout << "打开debug.txt文件错误" << endl;
        exit(0);
    }
    return outf;
}

inline void alert_content() {};

template <typename T, typename... Args>
inline void alert_content(const T& value, const Args&... args) {
    DEBUG_OPEN_FILE_INSTR;
    outf << value;
    DEBUG_CLOSE_FILE_INSTR;
    alert_content(args...);
}

inline void function_call_line() {
    DEBUG_OPEN_FILE_INSTR;
    for (int i = 0; i < debug_fuction_call_order; i++) {
        outf << FUNC_CALL_LINE;
    }
    DEBUG_CLOSE_FILE_INSTR;
}

template <typename T, typename... Args>
inline void alert_line(const T& value, const Args&... args) {
    DEBUG_OPEN_FILE_INSTR;
    function_call_line();
    if (SHOW_IDX) {
        outf << debug_content_cnt++ << "-";
    } else {
        debug_content_cnt++;
    }
    outf << value;
    DEBUG_CLOSE_FILE_INSTR;
    alert_content(args...);
}

template <typename T, typename... Args>
inline void inner_alert_line(const T& value, const Args&... args) {
    DEBUG_OPEN_FILE_INSTR;
    if (SHOW_IDX) {
        outf << debug_content_cnt++ << "-";
    } else {
        debug_content_cnt++;
    }
    outf << value;
    DEBUG_CLOSE_FILE_INSTR;
    alert_content(args...);
}

inline void blank_line() {
    DEBUG_OPEN_FILE_INSTR;
    outf << "\n";
    DEBUG_CLOSE_FILE_INSTR;
}

inline void alert_title(string content) {
    DEBUG_OPEN_FILE_INSTR;
    string title_line = string(TITLE_LENGTH, '*');
    int space_left = (TITLE_LENGTH - 2 - content.size()) / 2;
    space_left =  MAX(0, space_left);
    int space_right = TITLE_LENGTH - 2 - space_left - content.size();
    space_right = MAX(0, space_right);
    string title_blank_line(TITLE_LENGTH - 2, ' ');
    title_blank_line = "*" + title_blank_line + "*\n";
    string left_space_s(space_left, ' ');
    string right_space_s(space_right, ' ');
    outf << title_line << endl;
    for (int iii = 0; iii < TITLE_BLANK_LINE; iii++) {
        outf << title_blank_line;
    }
    outf << "*" << left_space_s << content << right_space_s << "*" << endl;
    for (int iii = 0; iii < TITLE_BLANK_LINE; iii++) {
        outf << title_blank_line;
    }
    outf << title_line << endl;
    //outf << space_left << " " << space_right <<  " " << content.size() << endl;
    DEBUG_CLOSE_FILE_INSTR;
}

inline void debug_cnt_reset() {
    debug_title_cnt = 0;
    debug_content_cnt = 0;
}

inline void begin_function(string name, int entrance_id=0, int call_order=1) {
    function_call_line();
    debug_fuction_call_order += call_order;
    inner_alert_line("Begin Function:  ");
    if (!entrance_id) {
        alert_content(name, "()\n");
    } else {
        alert_content(name, "()----- ", entrance_id, "-----\n");
    }
}

inline void end_function(string name, int exit_id=0, int call_order=-1) {
    debug_fuction_call_order += call_order;
    function_call_line();
    inner_alert_line("End Function:  ");
    if (!exit_id) {
        alert_content(name ,"()\n");
    } else {
        alert_content(name, "()----- ", exit_id, " -----\n");
    }
}

#endif