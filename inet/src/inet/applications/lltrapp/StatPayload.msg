// FillPayload.msg
// 

cplusplus {{
// #include <array>
#include <vector>
}}


namespace inet;

cplusplus {{
typedef std::vector<int> Array6;
//typedef std::array<int, (3-1)*(3)> Array6;
//using Array6 = std::array<int, (3-1)*(3)>;
}}
class noncobject Array6;

//
// TODO generated message class
//
packet StatPayload {
    Array6 fillCount;
}
