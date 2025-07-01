#ifndef D_A_OBJ_MKIEK_H
#define D_A_OBJ_MKIEK_H

#include "d/d_bg_s_movebg_actor.h"
#include "m_Do/m_Do_ext.h"

namespace daObjMkiek {
    class Act_c : public dBgS_MoveBgActor {
    public:
        void prm_get_sound() const {}
        void prm_get_swSave() const {}
    
        virtual BOOL CreateHeap();
        virtual BOOL Create();
        cPhs_State Mthd_Create();
        virtual BOOL Delete();
        BOOL Mthd_Delete();
        void set_mtx();
        void init_mtx();
        void check();
        void demo_wait();
        void demo();
        virtual BOOL Execute(Mtx**);
        virtual BOOL Draw();

        enum Prm_e {
            PRM_SWITCH_W = 0x08,
            PRM_SWITCH_S = 0x00,

            PRM_UNKNOWN_W = 0x01,   // TODO: find names
            PRM_UNKNOWN_S = 0x08
        };

        static const char M_arcname[];

   private:
        static Mtx M_tmp_mtx;        
    
    public:
        /* 0x2C8 */ request_of_phase_process_class mPhs;
        /* 0x2D0 */ J3DModel* mpModel;
        /* 0x2D4 */ J3DModel* mpModelV;
        /* 0x2D8 */ mDoExt_brkAnm mBrkAnm;
        /* 0x2F0 */ dCcD_Stts mStts;
        /* 0x32C */ dCcD_Sph mSph;
        /* 0x458 */ bool m458;
        /* 0x459 */ u8 unmatched1; // TODO: match
        /* 0x45A */ s16 mDieEventIdx;
        /* 0x45C */ s32 m45C;
        /* 0x460 */ s32 m460;
    };
};

#endif /* D_A_OBJ_MKIEK_H */
