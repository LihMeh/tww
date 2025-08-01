//
// Generated by dtk
// Translation Unit: d_a_obj_quake.cpp
//

#include "d/actor/d_a_obj_quake.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_a_obj.h"
#include "d/d_com_inf_game.h"

static daObjQuake_HIO_c l_HIO;

/* 000000EC-00000134       .text __ct__16daObjQuake_HIO_cFv */
daObjQuake_HIO_c::daObjQuake_HIO_c() {
    mNo = -1;
    field_0x08 = 0;
    field_0x0C = 0;
    field_0x0D = 0;
    field_0x0E = 3;
    field_0x0F = 3;
    field_0x10 = 1;
    field_0x11 = 1;
    field_0x12 = 1;
    field_0x13 = 0;
}

/* 00000134-000002C4       .text _create__12daObjQuake_cFv */
cPhs_State daObjQuake_c::_create() {
    fopAcM_SetupActor(this, daObjQuake_c);

    if (dComIfGs_isSymbol(1)) {
        return cPhs_STOP_e;
    }

    if (getPrmType() < 3) {
        fopAcM_offDraw(this);
        mType = getPrmType();
        m2A1 = false;
        m290 = 30.0f * dStage_stagInfo_GetSchSec(dComIfGp_getStageStagInfo());

        switch (mType) {
        case 0:
        case 2:
            m298 = 4.0f;
            m29C = 6.0f;
            break;
        case 1:
            m298 = 0.0f;
            m29C = 6.0f;
            break;
        }

        if (l_HIO.mNo < 0) {
            l_HIO.mNo = mDoHIO_createChild("振動効果", &l_HIO); // "Vibration Effect"
        }

        l_HIO.field_0x08 += 1;
        return cPhs_COMPLEATE_e;
    }

    return cPhs_ERROR_e;
}

/* 000002C4-000002CC       .text _delete__12daObjQuake_cFv */
BOOL daObjQuake_c::_delete() {
    return TRUE;
}

/* 000002CC-00000390       .text _is_delete__12daObjQuake_cFv */
BOOL daObjQuake_c::_is_delete() {
    if (m2A1) {
        switch (mType) {
        case 0:
            dComIfGp_getVibration().StopQuake(0x3E);
            break;
        case 2:
            dComIfGp_getVibration().StopQuake(1);
            break;
        }
    }

    if (l_HIO.mNo >= 0) {
        l_HIO.field_0x08 -= 1;
        if (l_HIO.field_0x08 == 0) {
            mDoHIO_deleteChild(l_HIO.mNo);
            l_HIO.mNo = -1;
        }
    }

    return TRUE;
}

/* 00000390-00000674       .text _execute__12daObjQuake_cFv */
BOOL daObjQuake_c::_execute() {
    // Required for match
    const char* unused[] = {"camera", "sound", "motor"}; // Has to be in a function after _create and before getPrmType

    m294 = dKy_get_schbit_timer();
    s32 var_r30 = 0x2E;
    s32 var_r29 = getPrmPower();
    s32 var_r28 = getPrmPower();

    if (l_HIO.field_0x0C != 0) {
        var_r29 = l_HIO.field_0x0E + 1;
    }

    if (l_HIO.field_0x0D != 0) {
        var_r28 = l_HIO.field_0x0F + 1;

        if (l_HIO.field_0x10 != 0) {
            var_r30 = var_r30 | 2;
        } else {
            var_r30 = var_r30 & ~2;
        }

        if (l_HIO.field_0x11 != 0) {
            var_r30 = var_r30 | 4;
        } else {
            var_r30 = var_r30 & ~4;
        }

        if (l_HIO.field_0x12 != 0) {
            var_r30 = var_r30 | 8;
        } else {
            var_r30 = var_r30 & ~8;
        }

        if (l_HIO.field_0x13 != 0) {
            var_r30 = var_r30 | 0x10;
        } else {
            var_r30 = var_r30 & ~0x10;
        }
    }

    u8 sch = getPrmSch();
    bool var_r27 = false;
    if (sch & dKy_get_schbit() && !dComIfGp_event_chkEventFlag(dEvtFlag_TALK_e)) {
        f32 a = (m29C / 6.0f) * m290;
        f32 b = (m298 / 6.0f) * m290;
        if (m294 >= b && m294 < a) {
            var_r27 = true;
        }
    }

    if (m2A1) {
        if (mType == 1) {
            mDoAud_seStart(JA_SE_ATM_JINARI);
        }

        if (!var_r27) {
            switch (mType) {
            case 0:
                dComIfGp_getVibration().StopQuake(var_r30);
                break;
            case 2:
                dComIfGp_getVibration().StopQuake(1);
                break;
            }
            m2A1 = false;
        }
    } else if (var_r27) {
        switch (mType) {
        case 0:
            dComIfGp_getVibration().StartQuake(var_r28, var_r30, cXyz(0.0f, 1.0f, 0.0f));
            break;
        case 2:
            dComIfGp_getVibration().StartQuake(var_r29, 1, cXyz(0.0f, 1.0f, 0.0f));
            break;
        }

        m2A1 = true;
    }

    return TRUE;
}

/* 00000674-000006A0       .text getPrmType__12daObjQuake_cFv */
u8 daObjQuake_c::getPrmType() {
    return daObj::PrmAbstract<Prm_e>(this, PRM_TYPE_W, PRM_TYPE_S);
}

/* 000006A0-000006CC       .text getPrmSch__12daObjQuake_cFv */
u8 daObjQuake_c::getPrmSch() {
    return daObj::PrmAbstract<Prm_e>(this, PRM_SCH_W, PRM_SCH_S);
}

/* 000006CC-000006F8       .text getPrmPower__12daObjQuake_cFv */
s32 daObjQuake_c::getPrmPower() {
    return daObj::PrmAbstract<Prm_e>(this, PRM_POWER_W, PRM_POWER_S) + 1;
}

/* 000006F8-00000718       .text daObjQuakeCreate__FPv */
static cPhs_State daObjQuakeCreate(void* i_this) {
    return static_cast<daObjQuake_c*>(i_this)->_create();
}

/* 00000718-00000738       .text daObjQuakeDelete__FPv */
static BOOL daObjQuakeDelete(void* i_this) {
    return static_cast<daObjQuake_c*>(i_this)->_delete();
}

/* 00000738-00000758       .text daObjQuakeExecute__FPv */
static BOOL daObjQuakeExecute(void* i_this) {
    return static_cast<daObjQuake_c*>(i_this)->_execute();
}

/* 00000758-00000760       .text daObjQuakeDraw__FPv */
static BOOL daObjQuakeDraw(void* i_this) {
    return TRUE;
}

/* 00000760-00000780       .text daObjQuakeIsDelete__FPv */
static BOOL daObjQuakeIsDelete(void* i_this) {
    return static_cast<daObjQuake_c*>(i_this)->_is_delete();
}

static actor_method_class daObjQuakeMethodTable = {
    (process_method_func)daObjQuakeCreate,
    (process_method_func)daObjQuakeDelete,
    (process_method_func)daObjQuakeExecute,
    (process_method_func)daObjQuakeIsDelete,
    (process_method_func)daObjQuakeDraw,
};

actor_process_profile_definition g_profile_Obj_Quake = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0002,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Quake,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjQuake_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Quake,
    /* Actor SubMtd */ &daObjQuakeMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
