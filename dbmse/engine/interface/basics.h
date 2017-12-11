// Toy DBMS, v0.4
// George Chernishev (c) 2016-2017, chernishev<at>google mail
// A task framework for undergraduate students at Saint-Petersburg Academic University, DBMS development course
// More details regarding the course can be found here: www.math.spbu.ru/user/chernishev/
// CHANGELOG:
// 0.4: no chance for efficiency competition, so, this year I reoriented task towards ideas:
//      1) tried to remove C code, now we are using lots of std:: stuff
//      2) implemented support for multiple attributes in the DBMS
//      3) code clean-up and restructurization
// 0.3: added:
//      1) support for restricting physical joins node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for PredicateInfo and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <bits/unique_ptr.h>


const int MAXLEN = 1000;

enum ValueType : short {
    VT_INT,
    VT_STRING
};

enum COLUMN_SORT {
    CS_ASCENDING,
    CS_DESCENDING,
    CS_NO,
    CS_UNKNOWN
};

struct Value {
    ValueType vtype;
    int vint;
    std::string vstr;

    Value(int v) {
        vtype = VT_INT;
        vint = v;
        vstr = "";
    }

    Value(std::string v) {
        vtype = VT_STRING;
        vstr = v;
        vint = 0;
    }

    Value(const Value &v) : Value(v.vtype, v.vint, v.vstr) {}

    Value(ValueType vtype, int vint, std::string vstr) : vtype(vtype), vint(vint), vstr(vstr) {}

    Value() {
        vtype = VT_INT;
        vint = 0;
        vstr = "";
    }

    explicit operator int() const { return vint; }

    explicit operator std::string() const { return vstr; }

    ~Value() = default;

    bool operator==(const Value &right) const {
        if (vtype != right.vtype) {
            throw std::runtime_error("Cannot compare Values of different types!");
        }

        switch (vtype) {
            case VT_INT:
                return vint == right.vint;
            case VT_STRING:
                return vstr == right.vstr;
        }

        throw std::runtime_error("Cannot get here!");
    }

    bool operator!=(const Value &right) const {
        return !(*this == right);
    }

    bool operator<(const Value &right) const {
        if (vtype != right.vtype) {
            throw std::runtime_error("Cannot compare Values of different types!");
        }

        switch (vtype) {
            case VT_INT:
                return vint < right.vint;
            case VT_STRING:
                return vstr < right.vstr;
        }

        throw std::runtime_error("Cannot get here!");
    }

    bool operator>(const Value &right) const {
        return right < *this;
    }

    bool operator>=(const Value &right) const {
        return !(*this < right);
    }

    bool operator<=(const Value &right) const {
        return !(*this > right);
    }

};

typedef std::vector<Value> query_result_row;
typedef std::vector<query_result_row> query_result;

typedef std::vector<std::string> name_aliases;
typedef std::pair<size_t, std::string> join_offset;

enum PredicateType {
    PT_EQUALS,
    PT_GREATERTHAN,
};

struct Predicate {
    virtual bool Apply(const query_result_row &row) const = 0;
};

struct EqualsPredicate : Predicate {

    EqualsPredicate(size_t attribute, const Value &value) : attribute(attribute), value(value) {}

    bool Apply(const query_result_row &row) const override {
        return row[attribute] == value;
    }

private:
    size_t attribute;
    Value value;
};

struct GreaterThanPredicate : Predicate {

    GreaterThanPredicate(size_t attribute, const Value &value) : attribute(attribute), value(value) {}

    bool Apply(const query_result_row &row) const override {
        return row[attribute] >= value;
    }

private:
    size_t attribute;
    Value value;
};

struct PredicateInfo {
    PredicateType ptype;
    int attribute;
    Value value;

    PredicateInfo(PredicateType ptype, int attribute, Value v)
        : ptype(ptype),
          attribute(attribute),
          value(v) {}

    PredicateInfo(const PredicateInfo &p) : PredicateInfo(p.ptype, p.attribute, p.value) {}

    PredicateInfo() {}

    ~PredicateInfo() {}

    std::unique_ptr<Predicate> ToPredicate() const {
        switch (ptype) {
            case PT_GREATERTHAN:
                return std::make_unique<GreaterThanPredicate>(attribute, value);
            case PT_EQUALS:
                return std::make_unique<EqualsPredicate>(attribute, value);
        }

        throw std::runtime_error("Unknown predicate type!");
    }

};

inline std::ostream &operator<<(std::ostream &stream, const PredicateInfo &p) {
    stream << "[" << p.attribute << "]";
    if (p.ptype == PT_EQUALS)
        stream << " == ";
    else
        stream << " > ";

    if (p.value.vtype == VT_INT)
        stream << p.value.vint;
    else
        stream << p.value.vstr;
    return stream;
}

struct BaseTable {
    std::string relpath;
    int nbAttr;
    std::vector<ValueType> vtypes;
    std::vector<std::string> vnames;
    std::vector<COLUMN_SORT> vorders;

    BaseTable() {}

    BaseTable(std::string p) : relpath(p) {
        std::string line, word;
        std::ifstream fin(relpath.c_str());
        if (fin.is_open()) {
            fin >> nbAttr;
            // names
            getline(fin, line);
            getline(fin, line);
            std::istringstream iss(line, std::istringstream::in);
            while (iss >> word) {
                vnames.push_back(word);
            }
            // types
            getline(fin, line);
            std::istringstream iss2(line, std::istringstream::in);
            while (iss2 >> word) {
                if (word == "INT")
                    vtypes.push_back(VT_INT);
                else
                    vtypes.push_back(VT_STRING);
            }
            // order
            getline(fin, line);
            std::istringstream iss3(line, std::istringstream::in);
            while (iss3 >> word) {
                if (word == "ASCENDING")
                    vorders.push_back(CS_ASCENDING);
                else if (word == "DESCENDING")
                    vorders.push_back(CS_DESCENDING);
                else if (word == "UNKNOWN")
                    vorders.push_back(CS_UNKNOWN);
                else if (word == "UNKNOWN")
                    vorders.push_back(CS_NO);
            }

            fin.close();
        } else std::cout << "Unable to open file";
    }

    ~BaseTable() {}
};

inline std::ostream &operator<<(std::ostream &stream, const BaseTable &bt) {
    stream << "located in " << bt.relpath << " having " << bt.nbAttr << " following attributes:" << std::endl;
    for (int i = 0; i < bt.nbAttr; i++) {
        stream << i << ". " << bt.vnames[i] << " ";
        if (bt.vtypes[i] == VT_INT)
            stream << "INT ";
        else
            stream << "STR ";
        if (bt.vorders[i] == CS_ASCENDING)
            stream << "ASCENDING";
        else if (bt.vorders[i] == CS_DESCENDING)
            stream << "DESCENDING";
        else if (bt.vorders[i] == CS_UNKNOWN)
            stream << "UNKNOWN";
        else if (bt.vorders[i] == CS_NO)
            stream << "UNSORTED";
        stream << std::endl;
    }
    return stream;
}


enum ErrCode {
    EC_OK,
    EC_FINISH,
    EC_ERROR
};