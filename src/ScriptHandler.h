/* -*- C++ -*-
 *
 *  ScriptHandler.h - Script manipulation class
 *
 *  Copyright (c) 2001-2005 Ogapee (original ONScripter, of which this
 *  is a fork).
 *
 *  ogapee@aqua.dti2.ne.jp
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 */

#ifndef __SCRIPT_HANDLER_H__
#define __SCRIPT_HANDLER_H__

#include "defs.h"
#include "BaseReader.h"
#include "DirPaths.h"
#include "expression.h"

const int VARIABLE_RANGE = 4096;

class ScriptHandler {
public:
    bool is_ponscripter;

    enum encoding_t { UTF8, CP932 };
    Encoding *utf_encoding;
    struct ScriptFilename {
	typedef std::vector<ScriptFilename> vec;
	typedef vec::iterator iterator;
	pstring filename;
	int encryption;
	encoding_t _encoding; // prevent name collision
	ScriptFilename(const char* f, int e, encoding_t c)
	    : filename(f), encryption(e), _encoding(c) {}
	pstring to_string() const { return filename; }
    };
    ScriptFilename::vec script_filenames;
    
    enum { END_NONE  = 0,
           END_COMMA = 1 };
    struct LabelInfo {
	typedef std::vector<LabelInfo> vec;
	typedef vec::iterator iterator;
	typedef dictionary<pstring, iterator>::t dic;
	pstring name;
        const char* label_header;
        const char* start_address;
        int start_line;
        int num_of_lines;
	LabelInfo() : start_address(NULL) {}
    };

    class ArrayVariable {
    public:
	typedef std::map<int, ArrayVariable> map;
	typedef map::iterator iterator;

	int  getValue(const h_index_t& i)          { return getoffs(i); }
	void setValue(const h_index_t& i, int val) { getoffs(i) = val; }

	h_index_t::size_type dimensions() const { return dim.size(); }
	int dimension_size(int depth)   const { return dim[depth]; }
	
	ArrayVariable(ScriptHandler* o, h_index_t sizes);

	h_index_t::iterator begin() { return data.begin(); }
	h_index_t::iterator end()   { return data.end(); }
    private:
	ScriptHandler* owner;
        h_index_t dim;
	h_index_t data;
	int& getoffs(const h_index_t& indices);
    };
    ArrayVariable::map arrays;

    enum { VAR_NONE  = 0,
           VAR_INT   = 1,  // integer
           VAR_ARRAY = 2,  // array
           VAR_STR   = 4,  // string
           VAR_CONST = 8,  // direct value or alias, not variable
           VAR_PTR   = 16, // pointer to a variable, e.g. i%0, s%0
	   VAR_LABEL = 32  // label, not string constant
    };
    struct VariableInfo {
        int type;
        int var_no;   // for integer(%), array(?), string($) variable
        h_index_t array; // for array(?)
	VariableInfo() {}
    };

    ScriptHandler();
    ~ScriptHandler();

    void reset();
    FILE *fileopen(const pstring& path, const char *mode, const bool save = false, const bool usesavedir = false);
    FILE *fileopen(const pstring& root, const pstring& path, const char *mode);
    void setKeyTable(const unsigned char* key_table);

    void setSavedir(const pstring& dir);

    // basic parser function
    const char* readToken(bool no_kidoku = false);
private:
    const char* readLabel();
    const char* readStr();
    int  readInt();
public:    
    int  parseInt(const char** buf);
    void skipToken();

    // saner parser functions :)
    // Implementations in expression.cpp
    bool hasMoreArgs();
    pstring readStrValue();
    pstring readBareword();
    int readIntValue();
    Expression readStrExpr(bool trace = false);
    Expression readIntExpr();
    Expression readExpr();
    char checkPtr();

    // function for string access
    inline pstring& getStrBuf() {
	return string_buffer;
    }
    inline const char* getStrBuf(int offset) {
	if (offset < 0 || offset > string_buffer.length()) {
	    fprintf(stderr, "getStrBuf outside buffer (offs %d, len %u)\n",
		    offset, string_buffer.length());
	    if (offset < 0) offset = 0;
	    else offset = string_buffer.length() - 1;
	}
	return ((const char*)string_buffer) + offset;
    }
    inline char readStrBuf(int offset) {
	if (offset < 0 || offset > string_buffer.length()) {
	    fprintf(stderr, "readStrBuf outside buffer (offs %d, len %u)\n",
		    offset, string_buffer.length());
	    return 0;
	}
	return string_buffer[offset];
    }
    void addStrBuf(char ch) { string_buffer += ch; }

    // function for direct manipulation of script address
    inline const char* getCurrent() { return current_script; };
    inline const char* getNext() { return next_script; };
    void setCurrent(const char* pos);
    void pushCurrent(const char* pos);
    void popCurrent();

    int getScriptBufferLength() const { return script_buffer_length; }
    
    int getOffset(const char* pos);
    const char* getAddress(int offset);
    int getLineByAddress(const char* address, bool absolute = false);
    const char* getAddressByLine(int line);
    LabelInfo getLabelByAddress(const char* address);
    LabelInfo getLabelByLine(int line);

    bool isText();
    //Mion: using these since 'isdigit' & 'isxdigit' behavior
    //      are locale-specific (at least on Windows)
    static inline bool isawspace(int c) { return ((c == ' ') || (c == '\t')); }
    static inline bool isadigit(int c) { return ((c >= '0') && (c <= '9')); }
    static inline bool isaxdigit(int c) {
        return (((c >= '0') && (c <= '9')) ||
                ((c >= 'a') && (c <= 'f')) ||
                ((c >= 'A') && (c <= 'F')));
    }

    bool compareString(const char* buf);

    void skipLine(int no = 1);
    void setLinepage(bool val);

    // function for kidoku history
    bool isKidoku();
    void markAsKidoku(const char* address = NULL);
    void setKidokuskip(bool kidokuskip_flag);
    void saveKidokuData();
    void loadKidokuData();

    void addStrVariable(const char** buf);
    void addIntVariable(const char** buf);
    void declareDim();

    void enableTextgosub(bool val);
    void setClickstr(pstring values);
    int  checkClickstr(const char* buf, bool recursive_flag = false);

    void setNumVariable(int no, int val);

    pstring stringFromInteger(int no, int num_column,
			     bool is_zero_inserted = false, bool do_wide = false);
    
    int readScriptSub(FILE* fp, char** buf, int encrypt_mode, bool is_utf=false);
    int readScript(DirPaths *path, const char* prefer_name);
    int labelScript();

    LabelInfo lookupLabel(const pstring& label);
    LabelInfo lookupLabelNext(const pstring& label);
    void errorAndExit(pstring str);
    void errorWarning(pstring str);

    void loadArrayVariable(FILE* fp);

    void addNumAlias(const pstring& str, int val)
	{ checkalias(str); num_aliases[str] = val; }
    void addStrAlias(const pstring& str, const pstring& val)
	{ checkalias(str); str_aliases[str] = val; }


    class LogInfo {
	typedef set<pstring>::t logged_t;
	logged_t logged;
	typedef std::vector<const pstring*> ordered_t;
	ordered_t ordered;
    public:
	pstring filename;
	bool find(pstring what);
	void add(pstring what);
	void clear() { ordered.clear(); logged.clear(); }
	void write(ScriptHandler& h);
	void read(ScriptHandler& h);
    } label_log, file_log;

    /* ---------------------------------------- */
    /* Variable */
    struct VariableData {
    private:
        int num;
    public:
        ScriptHandler* owner;
        int watch_int_variable;
        pstring str;
        bool num_limit_flag;
        int num_limit_upper;
        int num_limit_lower;

        int get_num() const {
            return num;
        }
        void set_num(int val) {
            if (watch_int_variable >= 0)
                fprintf(stderr, "WATCH (line %d): %%%d: %d -> %d\n",
                        owner->getLineByAddress(owner->getCurrent(), true),
                        watch_int_variable, num, val);
            num = val;
        }

        VariableData() {
            watch_int_variable = -1;
            reset(true);
        };
        void reset(bool limit_reset_flag)
        {
            set_num(0);
            if (limit_reset_flag) num_limit_flag = false;
            if (str) str.trunc(0);
        };
    };
    VariableData &getVariableData(int no);

    VariableInfo current_variable;

    int screen_size;
    int res_multiplier;
    /// Which things should `res_multiplier` apply to?
    /// @Note When adding new enum values, order them by the amount of things `res_multiplier` should change (rather than trying to preserve old IDs) so that individual usages can depend on <= checks rather than individually checking enum values
    enum MultiplierStyle {
        UMINEKO = 0, ///< Apply multiplier to things identically to the build used for the Steam release of Umineko
        FULL    = 1, ///< Apply multiplier to all things (for maximum compatibility with nscripter upscales)
    };
    MultiplierStyle multiplier_style;
    enum {
        SCREEN_SIZE_640x480,
        SCREEN_SIZE_800x600,
        SCREEN_SIZE_400x300,
        SCREEN_SIZE_320x240,
        SCREEN_SIZE_960x600,
        SCREEN_SIZE_w360,
        SCREEN_SIZE_w540,
        SCREEN_SIZE_w720,
        SCREEN_SIZE_w1080,
    };
    int global_variable_border;

    pstring game_identifier;
    pstring save_path;
    /// Alternative save directory used in local/steam mode (appended to exe directory)
    pstring local_savedir;
    //Mion: savedir is set by savedirCommand, stores save files
    // and main stored gamedata files except envdata
    pstring savedir;
    pstring script_path;

    static BaseReader* cBR;

private:
    enum { OP_INVALID = 0, // 000
           OP_PLUS    = 2, // 010
           OP_MINUS   = 3, // 011
           OP_MULT    = 4, // 100
           OP_DIV     = 5, // 101
           OP_MOD     = 6  // 110
    };

    LabelInfo::iterator findLabel(pstring label);

    const char* checkComma(const char* buf);
    pstring parseStr(const char** buf);
    int  parseIntExpression(const char** buf);
    void readNextOp(const char** buf, int* op, int* num);
    int  calcArithmetic(int num1, int op, int num2);
    typedef std::pair<int, h_index_t> array_ref;
    array_ref parseArray(const char** buf);
    
    /* ---------------------------------------- */
    /* Variable */
    std::vector<VariableData> variable_data;
    struct ExtendedVariableData{
        int no;
        VariableData vd;
        ExtendedVariableData(int n): no(n) {}
    };
    std::vector<ExtendedVariableData> extended_variable_data;

    typedef dictionary<pstring, int>::t    numalias_t;
    typedef dictionary<pstring, pstring>::t stralias_t;
    void checkalias(const pstring& alias);// warns if an alias may cause trouble
    numalias_t num_aliases;
    stralias_t str_aliases;

    DirPaths *archive_path;
    int   script_buffer_length;
    char* raw_script_buffer;
    char* script_buffer;
    char* tmp_script_buf;

    pstring string_buffer; // updated only by readToken (is this true?)

    LabelInfo::vec label_info;
    LabelInfo::dic label_names;
    
    bool  skip_enabled;
    bool  kidokuskip_flag;
    char* kidoku_buffer;

    bool  text_flag; // true if the current token is text
    int   end_status;
    bool  linepage_flag;
    bool  textgosub_flag;
    std::set<wchar> clickstr_list;

    const char* current_script;
    const char* next_script;

    const char* pushed_current_script;
    const char* pushed_next_script;

    unsigned char key_table[256];
    bool key_table_flag;
};

#endif // __SCRIPT_HANDLER_H__
