/**
 * d_a_swtact.cpp
 * Object - Wind Crest (Wind's Requiem blue floor decoration)
 */

#include "d/actor/d_a_swtact.h"
#include "d/res/res_itact.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/actor/d_a_player.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/JUtility/JUTAssert.h"

const char * daSwTact_c::m_arcname = "Itact";
const f32 daSwTact_c::mDefaultR = 50.0f;
const f32 daSwTact_c::mDefaultRwM = 100.0f;
const u32 daSwTact_c::m_heapsize = 0x3000;

/* 00000078-000000AC       .text _delete__10daSwTact_cFv */
bool daSwTact_c::_delete() {
    dComIfG_resDelete(&mPhs, m_arcname);
    return true;
}

/* 000000AC-000000CC       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_ac) {
    return ((daSwTact_c *)i_ac)->CreateHeap();
}

/* 000000CC-00000194       .text CreateHeap__10daSwTact_cFv */
BOOL daSwTact_c::CreateHeap() {
    J3DModelData * modelData = (J3DModelData *)dComIfG_getObjectRes(m_arcname, ITACT_BDL_ITACT);
    JUT_ASSERT(0xe1, modelData != NULL);
    model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000022);
    if (model == NULL)
        return FALSE;
    return TRUE;
}

/* 00000194-0000024C       .text CreateInit__10daSwTact_cFv */
void daSwTact_c::CreateInit() {
    static const f32 cull_size[] = {
        -120.0f, -10.0f, -120.0f,
        120.0f, 10.0f, 120.0f,
    };

    mSwitchNo = daSwTact_prm::getSwitchNo(this);
    mAnswer = daSwTact_prm::getAnswer(this);
    if (daSwTact_prm::getModel(this) == 1) {
        fopAcM_SetMtx(this, model->getBaseTRMtx());
        f32 scaleZ = scale.z;
        f32 scaleX = scale.x;
        fopAcM_setCullSizeBox(this, cull_size[0] * scaleX, cull_size[1], cull_size[2] * scaleZ, cull_size[3] * scaleX, cull_size[4], cull_size[5] * scaleZ);
        mRadius = 100.0f;
    } else {
        mRadius = 50.0f;
    }
}

/* 0000024C-000002CC       .text set_mtx__10daSwTact_cFv */
void daSwTact_c::set_mtx() {
    if (daSwTact_prm::getModel(this) == 1) {
        model->setBaseScale(scale);
        mDoMtx_stack_c::transS(current.pos);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

/* 000002CC-0000038C       .text _create__10daSwTact_cFv */
cPhs_State daSwTact_c::_create() {
    fopAcM_SetupActor(this, daSwTact_c);

    s32 result = cPhs_COMPLEATE_e;
    if (daSwTact_prm::getModel(this) == 1) {
        result = dComIfG_resLoad(&mPhs, m_arcname);

        if (result == cPhs_COMPLEATE_e) {
            if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, m_heapsize)) {
                return cPhs_ERROR_e;
            }
        }
    }

    if (result == cPhs_COMPLEATE_e) {
        CreateInit();
    }

    return result;
}

/* 0000038C-00000410       .text getAnswer__10daSwTact_cFv */
s32 daSwTact_c::getAnswer() {
    switch (mAnswer) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 5;
    case 3:
        return 2;
    case 4:
        return 3;
    case 5:
        return 4;
    case 0xFF:
    default:
        return -1;
    }
}

/* 00000410-000006AC       .text _execute__10daSwTact_cFv */
bool daSwTact_c::_execute() {
    daPy_py_c * player = daPy_getPlayerActorClass();
    u32 stts1 = dComIfGp_checkPlayerStatus1(0, daPyStts1_WIND_WAKER_CONDUCT_e);

    if (player == NULL || dComIfGp_checkPlayerStatus0(0, daPyStts0_SHIP_RIDE_e))
        return true;

    if ((player->current.pos - current.pos).absXZ() <= getR()) {
        if (mPlayerStatus != stts1 && stts1 != 0) {
            player->setTactZev(fopAcM_GetID(this), getAnswer(), NULL);
        }

        s32 tactMusic = player->getTactMusic();
        switch (mAnswer) {
        case 0:
            if (tactMusic == 0)
                mTrigger = true;
            break;
        case 1:
            if (tactMusic == 1)
                mTrigger = true;
            break;
        case 2:
            if (tactMusic == 5)
                mTrigger = true;
            break;
        case 3:
            if (tactMusic == 2)
                mTrigger = true;
            break;
        case 4:
            if (tactMusic == 3)
                mTrigger = true;
            break;
        case 5:
            if (tactMusic == 4)
                mTrigger = true;
            break;
        case 0xFF:
            if (tactMusic == 0 || tactMusic == 1 || tactMusic == 2 || tactMusic == 3 || tactMusic == 4 || tactMusic == 5)
                mTrigger = true;
            break;
        }
    }

    if (mTrigger != 0 && !dComIfGp_event_runCheck()) {
        dComIfGs_onSwitch(mSwitchNo, fopAcM_GetHomeRoomNo(this));
        mTrigger = 0;
    }

    mPlayerStatus = stts1;
    set_mtx();

    return true;
}

/* 000006AC-00000720       .text _draw__10daSwTact_cFv */
bool daSwTact_c::_draw() {
    if (daSwTact_prm::getModel(this) == 0)
        return TRUE;

    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    mDoExt_modelUpdateDL(model);
    return TRUE;
}

/* 00000720-00000740       .text daSwTact_Create__FPv */
static cPhs_State daSwTact_Create(void* i_ac) {
    return ((daSwTact_c *)i_ac)->_create();
}

/* 00000740-00000764       .text daSwTact_Delete__FPv */
static BOOL daSwTact_Delete(void* i_ac) {
    return ((daSwTact_c*)i_ac)->_delete();
}

/* 00000764-00000788       .text daSwTact_Draw__FPv */
static BOOL daSwTact_Draw(void* i_ac) {
    return ((daSwTact_c*)i_ac)->_draw();
}

/* 00000788-000007AC       .text daSwTact_Execute__FPv */
static BOOL daSwTact_Execute(void* i_ac) {
    return ((daSwTact_c*)i_ac)->_execute();
}

/* 000007AC-000007B4       .text daSwTact_IsDelete__FPv */
static BOOL daSwTact_IsDelete(void* i_ac) {
    return TRUE;
}

static actor_method_class daSwTactMethodTable = {
    (process_method_func)daSwTact_Create,
    (process_method_func)daSwTact_Delete,
    (process_method_func)daSwTact_Execute,
    (process_method_func)daSwTact_IsDelete,
    (process_method_func)daSwTact_Draw,
};

actor_process_profile_definition g_profile_SW_TACT = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_SW_TACT,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daSwTact_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_SW_TACT,
    /* Actor SubMtd */ &daSwTactMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
