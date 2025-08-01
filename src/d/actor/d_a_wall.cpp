/**
 * d_a_wall.cpp
 * Object - Bombable walls
 */

#include "d/actor/d_a_wall.h"
#include "d/res/res_hbw1.h"
#include "d/res/res_htw1.h"
#include "d/res/res_hbw2.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_mtx.h"

#include "weak_data_1811.h" // IWYU pragma: keep

const s16 daWall_c::m_heapsize[3] = {
    0x1160,
    0x1DD0,
    0x2000,
};

const s16 daWall_c::m_bmdname[3] = {HBW1_BDL_HBW1, HTW1_BDL_HTW1, HBW2_BDL_HBW2};
const s16 daWall_c::m_dzbname[3] = {HBW1_DZB_HBW1, HTW1_DZB_HTW1, HBW2_DZB_HBW2};

const Vec daWall_c::m_tri_vtx[3][4] = {
    {{150.0f, -120.0f, 0.0f},
     {-150.0f, -120.0f, 0.0f},
     {-150.0f, 120.0f, 0.0f},
     {150.0f, 120.0f, 0.0f}},
    {{700.0f, -500.0f, 0.0f},
     {-700.0f, -500.0f, 0.0f},
     {-700.0f, 500.0f, 0.0f},
     {700.0f, 500.0f, 0.0f}},
    {{180.0f, -90.0f, 0.0f},
     {-180.0f, -90.0f, 0.0f},
     {-180.0f, 90.0f, 0.0f},
     {180.0f, 90.0f, 0.0f}},
};

const Vec daWall_c::m_cull_size[][2] = {
    {{-180.0f, -150.0f, -50.0f}, {180.0f, 150.0f, 50.0f}},
    {{-700.0f, -600.0f, -100.0f}, {700.0f, 600.0f, 100.0f}},
    {{-200.0f, -100.0f, -50.0f}, {200.0f, 100.0f, 50.0f}},
};

const char* daWall_c::m_arcname[3] = {
    "Hbw1",
    "Htw1",
    "Hbw2",
};

GXColor daWall_c::m_smoke_color = { 0x86, 0x66, 0x6D, 0xC8 };

static dCcD_SrcTri l_tri_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ AT_TYPE_BOMB,
        /* SrcObjTg  SPrm    */ dCcG_TgSPrm_Shield_e | dCcG_TgSPrm_ShieldFrontRange_e,
        /* SrcObjCo  SPrm    */ 0,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ 0,
        /* SrcGObjAt Spl     */ 0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ 0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoHitMark_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGTriS
    {
        /* a */ 0.0f, 0.0f, 0.0f,
        /* b */ 0.0f, 0.0f, 0.0f,
        /* c */ 0.0f, 0.0f, 0.0f,
    },
};

/* 00000078-00000100       .text _delete__8daWall_cFv */
bool daWall_c::_delete() {
    mSmokeCb.end();
    if (heap != NULL && mState == false)
        dComIfG_Bgsp()->Release(mpBgW);

    dComIfG_resDelete(&mPhs, m_arcname[mType]);
    return TRUE;
}

/* 00000100-00000120       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daWall_c*)i_this)->CreateHeap();
}

/* 00000120-00000288       .text CreateHeap__8daWall_cFv */
BOOL daWall_c::CreateHeap() {
    J3DModelData* modelData =
        (J3DModelData*)(dComIfG_getObjectRes(m_arcname[mType], m_bmdname[mType]));
    JUT_ASSERT(0x181, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000022);
    if (mpModel == NULL)
        return FALSE;

    setMoveBGMtx();
    mpBgW = new dBgW();

    if (mpBgW) {
        bool error = mpBgW->Set((cBgD_t*)dComIfG_getObjectRes(m_arcname[mType], m_dzbname[mType]),
                                cBgW::MOVE_BG_e, &mMtx);
        return error != true ? TRUE : FALSE;
    }

    return false;
}

/* 00000288-00000380       .text CreateInit__8daWall_cFv */
void daWall_c::CreateInit() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    Vec cullMin = m_cull_size[mType][0];
    Vec cullMax = m_cull_size[mType][1];
    fopAcM_setCullSizeBox(this, cullMin.x, cullMin.y, cullMin.z, cullMax.x, cullMax.y, cullMax.z);
    fopAcM_setCullSizeFar(this, 1.0f);
    mStts.Init(0xFF, 0xFF, this);
    set_tri();
    dComIfG_Bgsp()->Regist(mpBgW, this);
    set_mtx();
    mpBgW->Move();
    mSwitchNo = fopAcM_GetParam(this) & 0xFF;
    mState = false;
}

/* 00000380-000004EC       .text _create__8daWall_cFv */
cPhs_State daWall_c::_create() {
    fopAcM_SetupActor(this, daWall_c);
    mType = fopAcM_GetParam(this) >> 8;
    mSwitchNo = fopAcM_GetParam(this) & 0xFF;
    bool isSwitch = dComIfGs_isSwitch(mSwitchNo, fopAcM_GetHomeRoomNo(this));
    if (isSwitch || mSwitchNo == 0xff) {
        return cPhs_ERROR_e;
    }

    cPhs_State phase_state = dComIfG_resLoad(&mPhs, m_arcname[mType]);

    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, m_heapsize[mType])) {
            return cPhs_ERROR_e;
        }
        CreateInit();
    }

    return phase_state;
}

/* 00000800-00000880       .text set_mtx__8daWall_cFv */
void daWall_c::set_mtx() {
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000880-000008F0       .text setMoveBGMtx__8daWall_cFv */
void daWall_c::setMoveBGMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(fopAcM_GetShapeAngle_p(this)->y);
    mDoMtx_stack_c::scaleM(scale);
    mDoMtx_copy(mDoMtx_stack_c::get(), mMtx);
}

/* 000008F0-00000980       .text _execute__8daWall_cFv */
bool daWall_c::_execute() {
    typedef void (daWall_c::*procFunc)();
    static procFunc mode_proc[] = {
        &daWall_c::mode_wait,
        &daWall_c::mode_break,
    };

    (this->*mode_proc[mState])();
    return true;
}

/* 00000980-00000A58       .text mode_wait__8daWall_cFv */
void daWall_c::mode_wait() {
    for (int i = 0; i < 2; i++) {
        if (mTri[i].ChkTgHit()) {
            cCcD_Obj* at = mTri[i].GetTgHitObj();
            if (at != NULL && at->ChkAtType(AT_TYPE_BOMB)) {
                set_effect();
                set_se();
                break;
            } else {
                mTri[i].ClrTgHit();
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        dComIfG_Ccsp()->Set(&mTri[i]);
    }
}

/* 00000A58-00000B14       .text mode_break__8daWall_cFv */
void daWall_c::mode_break() {
    if (mBreakCounter != 0) {
        mBreakCounter++;
        switch (mType) {
        case 0:
        case 1:
        case 2: {
            if ((mBreakCounter > 10) && cLib_chaseF(&mDst, 0.0f, 2.2222223f)) {
                fopAcM_delete(this);
            }

            JPABaseEmitter* pEmitter = mSmokeCb.getEmitter();
            if (pEmitter != NULL) {
                pEmitter->setGlobalAlpha(mDst);
            }
            break;
        }
        default:
            fopAcM_delete(this);
            break;
        }
    }
    fopAcM_offDraw(this);
}

/* 00000B14-00000D84       .text set_tri__8daWall_cFv */
void daWall_c::set_tri() {
    for (int i = 0; i < 2; i++) {
        mTri[i].Set(l_tri_src);
        mTri[i].SetStts(&mStts);
    }

    Vec vertex[3];
    vertex[0] = m_tri_vtx[mType][0];
    vertex[1] = m_tri_vtx[mType][1];
    vertex[2] = m_tri_vtx[mType][2];
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);

    for (int i = 0; i < 3; i++) {
        mDoMtx_stack_c::multVec(&vertex[i], &vertex[i]);
    }
    mTri[0].setPos(&vertex[0], &vertex[1], &vertex[2]);

    vertex[0] = m_tri_vtx[mType][0];
    vertex[1] = m_tri_vtx[mType][2];
    vertex[2] = m_tri_vtx[mType][3];

    for (int i = 0; i < 3; i++) {
        mDoMtx_stack_c::multVec(&vertex[i], &vertex[i]);
    }
    mTri[1].setPos(&vertex[0], &vertex[1], &vertex[2]);
}

/* 00000D84-00000F74       .text set_effect__8daWall_cFv */
void daWall_c::set_effect() {
    u16 projection_id[] = {
        dPa_name::ID_SCENE_A16E,
        dPa_name::ID_SCENE_A170,
        dPa_name::ID_SCENE_A172,
    };
    u16 particle_id[] = {
        dPa_name::ID_SCENE_A16F,
        dPa_name::ID_SCENE_A171,
        dPa_name::ID_SCENE_A173,
    };

    csXyz local_28 = current.angle;
    local_28.y += 0x8000;
    mDst = 200.00f;

    switch (mType) {
    case 0:
    case 1:
    case 2:
        dComIfGp_particle_setProjection(projection_id[mType], &current.pos, &current.angle, NULL,
                                        0xFF, NULL, fopAcM_GetRoomNo(this), &tevStr.mColorK0,
                                        &tevStr.mColorK0);
        dComIfGp_particle_setProjection(projection_id[mType], &current.pos, &local_28, NULL, 0xFF,
                                        NULL, fopAcM_GetRoomNo(this), &tevStr.mColorK0,
                                        &tevStr.mColorK0);
        mpEmitter = dComIfGp_particle_set(particle_id[mType], &current.pos, &current.angle, NULL,
                                          mDst, &mSmokeCb, fopAcM_GetRoomNo(this));
        if (mpEmitter != NULL) {
            mpEmitter->becomeImmortalEmitter();
        }
        break;
    default:
        mpEmitter = NULL;
        break;
    }
    mBreakCounter = 1;
    mState = 1;
    dComIfG_Bgsp()->Release(mpBgW);
    if (mSwitchNo != 0xFF)
        dComIfGs_onSwitch(mSwitchNo, fopAcM_GetHomeRoomNo(this));
}

/* 00000F74-00000FE4       .text set_se__8daWall_cFv */
void daWall_c::set_se() {
    fopAcM_seStart(this, JA_SE_OBJ_BOMB_WALL_BRK, 0);
}

/* 00000FE4-00001044       .text _draw__8daWall_cFv */
bool daWall_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    return TRUE;
}

/* 00001044-00001064       .text daWall_Create__FPv */
static cPhs_State daWall_Create(void* i_this) {
    return ((daWall_c*)i_this)->_create();
}

/* 00001064-00001088       .text daWall_Delete__FPv */
static BOOL daWall_Delete(void* i_this) {
    return ((daWall_c*)i_this)->_delete();
}

/* 00001088-000010AC       .text daWall_Draw__FPv */
static BOOL daWall_Draw(void* i_this) {
    return ((daWall_c*)i_this)->_draw();
}

/* 000010AC-000010D0       .text daWall_Execute__FPv */
static BOOL daWall_Execute(void* i_this) {
    return ((daWall_c*)i_this)->_execute();
}

/* 000010D0-000010D8       .text daWall_IsDelete__FPv */
static BOOL daWall_IsDelete(void*) {
    return TRUE;
}

static actor_method_class daWallMethodTable = {
    (process_method_func)daWall_Create,
    (process_method_func)daWall_Delete,
    (process_method_func)daWall_Execute,
    (process_method_func)daWall_IsDelete,
    (process_method_func)daWall_Draw,
};

actor_process_profile_definition g_profile_WALL = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_WALL,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daWall_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_WALL,
    /* Actor SubMtd */ &daWallMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
