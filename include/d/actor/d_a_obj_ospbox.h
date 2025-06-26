#ifndef D_A_OBJ_OSPBOX_H
#define D_A_OBJ_OSPBOX_H

#include "SSystem/SComponent/c_bg_s_gnd_chk.h"
#include "d/d_bg_s_movebg_actor.h"

namespace daObjOspbox {
    class Act_c : public dBgS_MoveBgActor {
    public:
        void prm_get_itemNo() const {}
        void prm_get_spec() const {}
    
        virtual BOOL CreateHeap();
        virtual BOOL Create();
        cPhs_State Mthd_Create();
        virtual BOOL Delete();
        BOOL Mthd_Delete();
        void set_mtx();
        void init_mtx();
        void make_item();
        void eff_break();
        void sound_break();
        void set_ground();
        void init_ground();
        virtual BOOL Execute(Mtx**);
        virtual BOOL Draw();

        static const char M_arcname[];
        static const dCcD_SrcCyl M_cyl_src;

    private:
        static Mtx M_tmp_mtx;
    
    public:
        /* 0x2C8 */ u32 mNotMatched[2];                 // TODO: please match
        /* 0x2D0 */ J3DModel *mpModel;
        /* 0x2D4 */ dCcD_Stts mStts;
        /* 0x310 */ dCcD_Cyl mCyl;
        /* 0x440 */ cBgS_GndChk mGndChk;
        /* 0x478 */ u8 notMatched2[20];     // TODO: please match
        /* 0x494 */ f32 m494;
        /* 0x498 */ s16 m498;
    };
};

#endif /* D_A_OBJ_OSPBOX_H */
