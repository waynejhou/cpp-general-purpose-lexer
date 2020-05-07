#ifndef MARCOS_HPP
#define MARCOS_HPP

#define SHOULDNT_BE_CALLED(ret, name, args, reason) \
    ret name args { throw runtime_error("Function \"" #name "\" shouldn't be called, " #reason); }

#define wthrow(exception, wstring)                                                  \
    try                                                                             \
    {                                                                               \
        throw exception("");                                                        \
    }                                                                               \
    catch (exception & e)                                                           \
    {                                                                               \
        cerr << "terminate called after throwing an instance of '" #exception "'\n" \
             << "  what(): " #wstring << endl;                                      \
        abort();                                                                    \
    }

#define ANSI_ESC(code) "\e["#code"m"

#define ANSI_ESC_Reset "\e[0m"

#define ANSI_ESC_Bold       "\e[1m"
#define ANSI_ESC_Dim        "\e[2m"
#define ANSI_ESC_Underlined "\e[4m"
#define ANSI_ESC_Blink      "\e[5m"
#define ANSI_ESC_Reverse    "\e[7m"
#define ANSI_ESC_Hidden     "\e[8m"

#define ANSI_ESC_FG_DFT  "\e[39m"
#define ANSI_ESC_FG_BLK  "\e[30m"
#define ANSI_ESC_FG_RED  "\e[31m"
#define ANSI_ESC_FG_GRN  "\e[32m"
#define ANSI_ESC_FG_YEL  "\e[33m"
#define ANSI_ESC_FG_BLU  "\e[34m"
#define ANSI_ESC_FG_MGT  "\e[35m"
#define ANSI_ESC_FG_CYN  "\e[36m"
#define ANSI_ESC_FG_LGRY "\e[37m"
#define ANSI_ESC_FG_DGRY "\e[90m"
#define ANSI_ESC_FG_LRED "\e[91m"
#define ANSI_ESC_FG_LGRN "\e[92m"
#define ANSI_ESC_FG_LYEL "\e[93m"
#define ANSI_ESC_FG_LBLU "\e[94m"
#define ANSI_ESC_FG_LMGT "\e[95m"
#define ANSI_ESC_FG_LCYN "\e[96m"
#define ANSI_ESC_FG_WHT  "\e[97m"

#define ANSI_ESC_BG_DFT "\e[49m"
#define ANSI_ESC_BG_BLK "\e[40m"
#define ANSI_ESC_BG_RED "\e[41m"
#define ANSI_ESC_BG_GRN "\e[42m"
#define ANSI_ESC_BG_YEL "\e[43m"
#define ANSI_ESC_BG_BLU "\e[44m"
#define ANSI_ESC_BG_MGT "\e[45m"
#define ANSI_ESC_BG_CYN "\e[46m"
#define ANSI_ESC_BG_LGRY "\e[47m"
#define ANSI_ESC_BG_DGRY "\e[100m"
#define ANSI_ESC_BG_LRED "\e[101m"
#define ANSI_ESC_BG_LGRN "\e[102m"
#define ANSI_ESC_BG_LYEL "\e[103m"
#define ANSI_ESC_BG_LBLU "\e[104m"
#define ANSI_ESC_BG_LMGT "\e[105m"
#define ANSI_ESC_BG_LCYN "\e[106m"
#define ANSI_ESC_BG_WHT "\e[107m"
#endif