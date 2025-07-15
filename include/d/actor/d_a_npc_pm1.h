#ifndef D_A_NPC_PM1_H
#define D_A_NPC_PM1_H

#include "d/d_npc.h"

class daNpc_Pm1_c : public fopNpc_npc_c {
public:
    struct anm_prm_c {
        
    };

    void getAttPos() {}
    void getBackboneJntNum() {}
    void getBackbone_x() {}
    void getBackbone_y() {}
    void getEyePos() {}
    void getHeadJntNum() {}
    void getHead_x() {}
    void getHead_y() {}

    void createInit();
    void setMtx();
    void anmResID(int, int*, int*);
    void BtpNum2ResID(int, int*);
    void setAnm_tex(signed char);
    void init_btp(bool, int);
    void initTexPatternAnm(bool);
    void playTexPatternAnm();
    void setAnm_anm(anm_prm_c*);
    void setAnm();
    void chngAnmTag();
    void ctrlAnmTag();
    void chngAnmAtr(unsigned char);
    void ctrlAnmAtr();
    void setAnm_ATR(int);
    void anmAtr(unsigned short);
    void setStt(s8);
    u16 next_msgStatus(u32* pMsgNo);
    u32 getMsg();
    void eventOrder();
    void checkOrder();
    void lookBack();
    void chkAttention();
    void setAttention();
    void decideType(int);
    void event_actionInit(int);
    void event_action();
    void privateCut();
    void endEvent();
    void event_proc();
    void set_action(int (daNpc_Pm1_c::*)(void*), void*);
    void wait01();
    void talk01();
    void wait_action1(void*);
    void demo();
    BOOL _draw();
    BOOL _execute();
    BOOL _delete();
    cPhs_State _create();
    BOOL CreateHeap();

public:
    /* 0x6C4 */ request_of_phase_process_class mPhs;
    /* 0x6CC */ u8 field_0x6CC[0x700 - 0x6CC];  // TODO: match
    /* 0x700 */ dNpc_EventCut_c mEventCut;
    /* 0x76c */ u8 field_0x76C[0x78C - 0x76C];  // TODO: match
    /* 0x78C */ Vec field_0x78C;                // cXyz or Vec?
    /* 0x7BC */ u32 field_0x7BC;                // TODO: check type. Saved with stw, loaded with lwz.  is it signed or unsigned? 
    /* 0x7C0 */ u32 field_0x7C0;                // TODO: check type. Saved with stw, loaded with lwz.  is it signed or unsigned? 
    /* 0x7C5 */ u8 field_0x7C5;                 // TODO: check type. saved with stb in checkOrder ... is it signed or unsigned?
    /* 0x7CB */ u8 field_0x7CB;                 // TODO: check type. Saved with stb... but we save 0xFF ... may be, it's -1?
    /* 0x7CF */ s8 field_0x7CF;                 // TODO: check type. compared with CMPWI instead of CMPLWI in checkOrder , but saved with stb
    /* 0x7D0 */ s8 field_0x7D0;
    /* 0x7D1 */ s8 field_0x7D1;
    /* 0x7D2 */ u8 field_0x7D2;                 // TODO: check type. saved with stb, but s or u?
};

class daNpc_Pm1_HIO_c {
public:
    daNpc_Pm1_HIO_c();

public:
    /* 0x00 */ u8 field_0x00[0x20];             // TODO: match
    /* 0x20 */ f32 field_0x20;
};

#endif /* D_A_NPC_PM1_H */
