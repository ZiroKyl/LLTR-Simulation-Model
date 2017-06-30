// FillPayload.msg
// 

namespace inet;

cplusplus {{
enum LLTRStep: uint8_t {
	INIT = 0,
	PROBING,
	COLLECT
};
}}
class noncobject LLTRStep;

//
// TODO generated message class
//
packet FillPayload {
    LLTRStep step;
    uint32   iterationNumber;
}
