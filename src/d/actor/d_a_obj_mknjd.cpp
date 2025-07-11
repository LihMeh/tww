/**
 * d_a_obj_mknjd.cpp
 * Object - Earth God's Lyric/Wind God's Aria statues
 */

#include "d/actor/d_a_obj_mknjd.h"
#include "d/d_s_play.h" // IWYU pragma: keep
#include "d/res/res_mknjd.h"
#include "f_op/f_op_actor_mng.h"
#include "f_pc/f_pc_manager.h"
#include "f_op/f_op_msg.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JAZelAudio/JAIZelBasic.h"
#include "SSystem/SComponent/c_xyz.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_w.h"
#include "d/d_item.h"
#include "d/d_item_data.h"
#include "d/d_particle.h"
#include "d/actor/d_a_player.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"


#define ACT_SETGOAL 0
#define ACT_SETANGLE 1
#define ACT_WAIT 2
#define ACT_INPUT 3
#define ACT_BREAK 4
#define ACT_HIDE_LINK 5
#define ACT_DISP_LINK 6
#define ACT_LESSON 7
#define ACT_TACT 8

static const char* daObjMknjD_jointName[] = {
    "Hahen1",
    "Hahen2",
    "Hahen3",
    "Hahen4",
    "Hahen5",
    "Hahen6",
    "Hahen7",
    "Hahen8",
    "Hahen9",
    "Hahen10",
    "Hahen11",
    "Hahen12",
    "Hahen13",
    "Hahen14",
    "Hahen15",
    "Hahen16",
    "Hahen17",
    "Hahen18",
    "Hahen19",
    "Hahen20"
};

char* daObjMknjD_EventName[] = {
    "MKNJD_D_DEMO",
    "MKNJD_K_DEMO",

    "MKNJD_D_CHECK",
    "MKNJD_K_CHECK",

    "MKNJD_D_ERROR",
    "MKNJD_K_ERROR",

    "MKNJD_D_LESSON",
    "MKNJD_K_LESSON",
};

static u16 joint_number_table[20];

Mtx daObjMknjD::Act_c::M_tmp_mtx;

/* 00000078-0000012C       .text nodeCallBackL__FP7J3DNodei */
static BOOL nodeCallBackL(J3DNode* i_node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        int jntNo = static_cast<J3DJoint*>(i_node)->getJntNo();
        
        J3DModel* mdl = j3dSys.getModel();
        daObjMknjD::Act_c* actor = (daObjMknjD::Act_c*)mdl->getUserArea();

        if (actor != NULL) {
            MTXCopy(mdl->getAnmMtx(jntNo), *calc_mtx);

            MtxTrans(actor->mLeftHalfPos.x, actor->mLeftHalfPos.y, actor->mLeftHalfPos.z, 1);

            mdl->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);
        }
    }

    return TRUE;
}

/* 0000012C-000001E0       .text nodeCallBackR__FP7J3DNodei */
static BOOL nodeCallBackR(J3DNode* i_node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        int jntNo = static_cast<J3DJoint*>(i_node)->getJntNo();

        J3DModel* mdl = j3dSys.getModel();
        daObjMknjD::Act_c* actor = (daObjMknjD::Act_c*)mdl->getUserArea();

        if (actor != NULL) {
            MTXCopy(mdl->getAnmMtx(jntNo), *calc_mtx);

            MtxTrans(actor->mRightHalfPos.x, actor->mRightHalfPos.y, actor->mRightHalfPos.z, 1);

            mdl->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);
        }
    }

    return TRUE;
}

/* 000001E0-000002B0       .text nodeCallBack_Hahen__FP7J3DNodei */
static BOOL nodeCallBack_Hahen(J3DNode* i_node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        int jntNo = static_cast<J3DJoint*>(i_node)->getJntNo();
        u16 shardIdx = joint_number_table[jntNo - 1];

        J3DModel* mdl = j3dSys.getModel();
        daObjMknjD::Act_c* actor = (daObjMknjD::Act_c*)mdl->getUserArea();

        if (actor != NULL) {
            MTXCopy(mdl->getAnmMtx(jntNo), *calc_mtx);

            MtxTrans(actor->mShardPositions[shardIdx].x, actor->mShardPositions[shardIdx].y, actor->mShardPositions[shardIdx].z, 1);

            mdl->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);
        }
    }

    return TRUE;
}

/* 000002B0-000002D0       .text daObjMknjD_XyCheckCB__FPvi */
static s16 daObjMknjD_XyCheckCB(void* i_this, int i_param2) {
    return static_cast<daObjMknjD::Act_c*>(i_this)->XyCheckCB(i_param2);
}

/* 000002D0-000002F0       .text daObjMknjD_XyEventCB__FPvi */
static s16 daObjMknjD_XyEventCB(void* i_this, int i_param2) {
    return static_cast<daObjMknjD::Act_c*>(i_this)->XyEventCB(i_param2);
}

/* 000002F0-00000314       .text XyCheckCB__Q210daObjMknjD5Act_cFi */
s16 daObjMknjD::Act_c::XyCheckCB(int i_itemBtn) {
    if (dComIfGp_getSelectItem(i_itemBtn) == dItem_WIND_WAKER_e) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* 00000314-0000031C       .text XyEventCB__Q210daObjMknjD5Act_cFi */
s16 daObjMknjD::Act_c::XyEventCB(int) {
    return mLessonEventIdx;
}

/* 0000031C-00000620       .text CreateHeap__Q210daObjMknjD5Act_cFv */
BOOL daObjMknjD::Act_c::CreateHeap() {
    J3DModelData* model_data_d;
    if (m043E == true) {
        model_data_d = (J3DModelData*)dComIfG_getObjectRes(M_arcname, MKNJD_BDL_MKNJK);
    }
    else {
        model_data_d = (J3DModelData*)dComIfG_getObjectRes(M_arcname, MKNJD_BDL_MKNJD);
    }

    J3DModelData* model_data_h = (J3DModelData*)dComIfG_getObjectRes(M_arcname, MKNJD_BDL_MKNJH);

    JUT_ASSERT(0x123, model_data_d != NULL)
    JUT_ASSERT(0x124, model_data_h != NULL)
    
    mMainMdl = mDoExt_J3DModel__create(model_data_d, 0x80000, 0x31000002);
    mBreakMdl = mDoExt_J3DModel__create(model_data_h, 0x80000, 0x11000002);

    const char* jntName;
    if (mMainMdl != NULL && mBreakMdl != NULL) {
        JUTNameTab* nameTable = mMainMdl->getModelData()->getJointName();
        
        for (u16 i = 0; i < mMainMdl->getModelData()->getJointNum(); i++) {
            jntName = nameTable->getName(i);

            if (strcmp("MknjL", jntName) == 0) {
                mMainMdl->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBackL);
            }
            else if (strcmp("MknjR", jntName) == 0) {
                mMainMdl->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBackR);
            }
        }

        mMainMdl->setUserArea(reinterpret_cast<u32>(this));

        int curTblIdx = 0;
        nameTable = mBreakMdl->getModelData()->getJointName();

        for (u16 i = 0; i < mBreakMdl->getModelData()->getJointNum(); i++) {
            jntName = nameTable->getName(i);

            for (u16 j = 0; j < 20; j++) {
                if (strcmp(daObjMknjD_jointName[j], jntName) == 0) {
                    mBreakMdl->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_Hahen);
                    joint_number_table[curTblIdx++] = j;

                    break;
                }
            }
        }

        mBreakMdl->setUserArea(reinterpret_cast<u32>(this));
        mMainMdlAlpha = 0xFF;

        return TRUE;
    }

    return FALSE;
}

/* 00000620-000008E8       .text Create__Q210daObjMknjD5Act_cFv */
BOOL daObjMknjD::Act_c::Create() {
    fopAcM_SetMtx(this, mMainMdl->getBaseTRMtx());
    init_mtx();
    fopAcM_SetMtx(this, mBreakMdl->getBaseTRMtx());
    init_mtx();

    fopAcM_setCullSizeBox(this, -400.0f, -1.0f, -400.0f, 400.0f, 405.0f, 400.0f);

    mLeftHalfPos.setall(0.0f);
    mRightHalfPos.setall(0.0f);

    for (int i = 0; i < 20; i++) {
        mShardPositions[i].setall(0.0f);
        mShardHeights[i] = 0.0f;
    }

    m043D = false;
    mBreakTimer = 0;

    for (int i = 0; i < 4; i++) {
        mEmitters[i] = NULL;
#if VERSION == VERSION_DEMO
        mSmokeCBs[i] = dPa_smokeEcallBack(0);
#endif
    }

    if (m043E == true) {
        mCheckEventIdx = dComIfGp_evmng_getEventIdx(daObjMknjD_EventName[3]);
        mDemoEventIdx = dComIfGp_evmng_getEventIdx(daObjMknjD_EventName[1]);
        mErrorEventIdx = dComIfGp_evmng_getEventIdx(daObjMknjD_EventName[5]);
        mLessonEventIdx = dComIfGp_evmng_getEventIdx(daObjMknjD_EventName[7]);

        mMelodyNum = 4;
        mGiveItemNo = TACT_SONG5;
        eventInfo.setEventName("MKNJD_K_TALK");
        m0430 = 0x2910;
    }
    else {
        mCheckEventIdx = dComIfGp_evmng_getEventIdx(daObjMknjD_EventName[2]);
        mDemoEventIdx = dComIfGp_evmng_getEventIdx(daObjMknjD_EventName[0]);
        mErrorEventIdx = dComIfGp_evmng_getEventIdx(daObjMknjD_EventName[4]);
        mLessonEventIdx = dComIfGp_evmng_getEventIdx(daObjMknjD_EventName[6]);

        mMelodyNum = 3;
        mGiveItemNo = TACT_SONG4;
        eventInfo.setEventName("MKNJD_D_TALK");
        m0430 = 0x2920;
    }

    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0x3D;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0x3D;
    cLib_onBit<u32>(attention_info.flags, fopAc_Attn_ACTION_SPEAK_e | fopAc_Attn_TALKFLAG_CHECK_e);

    if (!checkItemGet(mGiveItemNo, 1)) {
        m043F = 8;
        eventInfo.setXyCheckCB(daObjMknjD_XyCheckCB);
        eventInfo.setXyEventCB(daObjMknjD_XyEventCB);
    }
    else {
        m043F = 0;
    }

    mMsgPtr = NULL;
    mMsgPID = fpcM_ERROR_PROCESS_ID_e;
    m0504 = false;

    return TRUE;
}

/* 000008E8-00000A84       .text Mthd_Create__Q210daObjMknjD5Act_cFv */
cPhs_State daObjMknjD::Act_c::Mthd_Create() {
    fopAcM_SetupActor(this, daObjMknjD::Act_c);

    m043E = prm_get_Type();

    cPhs_State phase_state;
    if (fopAcM_isSwitch(this, prm_get_swSave())) {
        mEmitters[2] = NULL;
        mEmitters[3] = NULL;

        return cPhs_STOP_e;
    }
    else {
        phase_state = dComIfG_resLoad(&mPhs, M_arcname);
        if (phase_state == cPhs_COMPLEATE_e) {
            phase_state = MoveBGCreate(M_arcname, MKNJD_DZB_MKNJD, NULL, 0x65A0);

            JUT_ASSERT(0x1CA, (phase_state == cPhs_COMPLEATE_e) || (phase_state == cPhs_ERROR_e))
        }
    }

    return phase_state;
}

/* 00000B64-00000BDC       .text Delete__Q210daObjMknjD5Act_cFv */
BOOL daObjMknjD::Act_c::Delete() {
#if VERSION == VERSION_DEMO
    if (mEmitters[2]) {
        mSmokeCBs[2].remove();
    }
    if (mEmitters[3]) {
        mSmokeCBs[3].remove();
    }
#else
    dComIfGp_att_revivalAleart();

    for (int i = 0; i < 4; i++) {
        mSmokeCBs[i].remove();
    }
#endif

    return TRUE;
}

/* 00000BDC-00000C34       .text Mthd_Delete__Q210daObjMknjD5Act_cFv */
BOOL daObjMknjD::Act_c::Mthd_Delete() {
    int bgDeleteResult = MoveBGDelete();
    
    if (fpcM_CreateResult(this) != cPhs_STOP_e) {
        dComIfG_resDeleteDemo(&mPhs, M_arcname);
    }

    return bgDeleteResult;
}

/* 00000C34-00000CC8       .text set_mtx__Q210daObjMknjD5Act_cFv */
void daObjMknjD::Act_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    mMainMdl->setBaseTRMtx(mDoMtx_stack_c::get());
    mBreakMdl->setBaseTRMtx(mDoMtx_stack_c::get());

    cMtx_copy(mDoMtx_stack_c::get(), M_tmp_mtx);
}

/* 00000CC8-00000D28       .text init_mtx__Q210daObjMknjD5Act_cFv */
void daObjMknjD::Act_c::init_mtx() {
    mMainMdl->setBaseScale(cXyz(1.0f, 1.0f, 1.0f));
    mBreakMdl->setBaseScale(cXyz(1.0f, 1.0f, 1.0f));

    set_mtx();
}

/* 00000D28-00000E04       .text setGoal__Q210daObjMknjD5Act_cFi */
void daObjMknjD::Act_c::setGoal(int i_staffIdx) {
    cXyz pos = *dComIfGp_evmng_getMyXyzP(i_staffIdx, "Posion");

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);

    mDoMtx_stack_c::transM(pos);
    
    mDoMtx_multVecZero(mDoMtx_stack_c::get(), &pos);
    mGoalPos = pos;

    dComIfGp_evmng_setGoal(&mGoalPos);
}

/* 00000E04-00000E84       .text setPlayerAngle__Q210daObjMknjD5Act_cFi */
void daObjMknjD::Act_c::setPlayerAngle(int i_staffIdx) {
    s16 angle = *dComIfGp_evmng_getMyIntegerP(i_staffIdx, "angle");
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    
    player->setPlayerPosAndAngle(
        &player->current.pos,
        current.angle.y + angle
    );
}

/* 00000E84-00000F88       .text talk__Q210daObjMknjD5Act_cFi */
u16 daObjMknjD::Act_c::talk(int i_param1) {
    u16 msgStatus = 0xFF;

    if (mMsgPID == fpcM_ERROR_PROCESS_ID_e) {
        if (i_param1 == 1) {
            mMsgNo = getMsg();
        }

        mMsgPID = fopMsgM_messageSet(mMsgNo, this);
        mMsgPtr = NULL;
    }
    else {
        if (mMsgPtr) {
            msgStatus = mMsgPtr->mStatus;

            if (msgStatus == fopMsgStts_MSG_DISPLAYED_e) {
                if (dComIfGp_checkMesgCancelButton() != 0) {
                    m0504 = true;
                    fopMsgM_messageSendOn();
                }

                mMsgPtr->mStatus = fopMsgStts_MSG_ENDS_e;
            }
            else if (msgStatus == fopMsgStts_BOX_CLOSED_e) {
                mMsgPtr->mStatus = fopMsgStts_MSG_DESTROYED_e;
                mMsgPID = fpcM_ERROR_PROCESS_ID_e;
            }
        }
        else {
            mMsgPtr = fopMsgM_SearchByID(mMsgPID);
        }
    }

    return msgStatus;
}

/* 00000F88-00001348       .text privateCut__Q210daObjMknjD5Act_cFv */
void daObjMknjD::Act_c::privateCut() {
    static char* cut_name_table[] = {
        "SETGOAL",
        "SETANGLE",
        "WAIT",
        "INPUT",
        "BREAK",
        "HIDE_LINK",
        "DISP_LINK",
        "LESSON",
        "TACT"
    };

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    int staffIdx = dComIfGp_evmng_getMyStaffId("MknjD");
    if (staffIdx != -1) {
        mActionIdx = dComIfGp_evmng_getMyActIdx(staffIdx, cut_name_table, ARRAY_SIZE(cut_name_table), TRUE, 0);

        if (mActionIdx == -1) {
            dComIfGp_evmng_cutEnd(staffIdx);
        }
        else {
            bool doCutEnd = false;
            
            if (dComIfGp_evmng_getIsAddvance(staffIdx)) {
                switch (mActionIdx) {
                    case ACT_SETGOAL:
                        setGoal(staffIdx);
                        break;
                    case ACT_SETANGLE:
                        setPlayerAngle(staffIdx);
                        break;
                    case ACT_BREAK:
                        mDoAud_seStart(JA_SE_READ_RIDDLE_1);

                        fopAcM_onSwitch(this, prm_get_swSave());

                        mBreakTimer = 0;

                        if (mpBgW != NULL) {
                            if (mpBgW->ChkUsed()) {
                                dComIfG_Bgsp()->Release(mpBgW);
                            }
                        }

                        break;
                    case ACT_HIDE_LINK:
                        player->onPlayerNoDraw();
                        break;
                    case ACT_DISP_LINK:
                        player->offPlayerNoDraw();
                        break;
                    case ACT_LESSON:
                        m0504 = false;
                        m0500 = 0;
                        mMsgPID = fpcM_ERROR_PROCESS_ID_e;
                        break;
                    case ACT_TACT:
                        break;
                }
            }

            switch (mActionIdx) {
                case ACT_SETGOAL:
                    doCutEnd = true;
                    break;
                case ACT_SETANGLE:
                    doCutEnd = true;
                    break;
                case ACT_INPUT:
                    if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
                        doCutEnd = true;
                    }
                    break;
                case ACT_WAIT:
                    doCutEnd = true;
                    break;
                case ACT_BREAK: {
                    if (daObjMknjD_break() == true) {
                        if (strcmp(dComIfGp_getStartStageName(), "Ekaze") == 0 || strcmp(dComIfGp_getStartStageName(), "Edaichi") == 0) {
                            mDoAud_bgmStart(JA_BGM_JABOO_CAVE);
                        }
                        else if (m043E == true) {
                            mDoAud_bgmStart(JA_BGM_D_WIND);
                        }
                        else {
                            mDoAud_bgmStart(JA_BGM_D_EARTH);
                        }

#if VERSION > VERSION_DEMO
                        if (mSmokeCBs[2].getEmitter() != NULL) {
                            mSmokeCBs[2].remove();
                        }
                        if (mSmokeCBs[3].getEmitter() != NULL) {
                            mSmokeCBs[3].remove();
                        }
#endif

                        doCutEnd = true;
                    }
                    break;
                }
                case ACT_LESSON: {
                    u16 msgStatus = talk(1);

                    if (msgStatus == fopMsgStts_BOX_CLOSED_e || msgStatus == fopMsgStts_UNK15_e) {
                        doCutEnd = true;
                    }
                    break;
                }
                case ACT_TACT:
                    if (m0504 == false) {
                        doCutEnd = true;
                    }
                    else {
                        dComIfGp_event_reset();
                    }
                    break;
                default:
                    doCutEnd = true;
                    break;
            }

            if (doCutEnd) {
                dComIfGp_evmng_cutEnd(staffIdx);
            }
        }
    }
}

/* 00001348-00001400       .text manage_friend_draw__10daObjMknjDFi */
void daObjMknjD::manage_friend_draw(int i_param1) {
    fopAc_ac_c* judgeResult = fopAcM_SearchByName(PROC_NPC_MD);

    if (judgeResult != NULL) {
        if (i_param1 == 1) {
            fopAcM_onDraw(judgeResult);
        } else {
            fopAcM_offDraw(judgeResult);
        }
    }

    judgeResult = fopAcM_SearchByName(PROC_NPC_CB1);
    
    if (judgeResult != NULL) {
        if (i_param1 == 1) {
            fopAcM_onDraw(judgeResult);
        } else {
            fopAcM_offDraw(judgeResult);
        }
    }
}

/* 00001400-0000195C       .text daObjMknjD_break__Q210daObjMknjD5Act_cFv */
bool daObjMknjD::Act_c::daObjMknjD_break() {
    bool ret = false;
    mBreakTimer++;

    /* Statue splitting into left/right halves */
    // From 0 to 60 frames, the left and right halves of the statue split apart.
    if (mBreakTimer < 60) {
        mLeftHalfPos.x = -((mBreakTimer * 20.0) / 60.0);
        mRightHalfPos.x = (mBreakTimer * 20.0) / 60.0;
    }
    // After 60 frames, the halves stay at +/- 20 units on the X axis.
    else {
        mLeftHalfPos.x = -20.0f;
        mRightHalfPos.x = 20.0f;
    }

    /* Particles and sound effects */
    // After 1 frame, the particles for the statue splitting in half spawn.
    if (mBreakTimer == 1) {
        mEmitters[0] = dComIfGp_particle_set(dPa_name::ID_SCENE_8185, &current.pos, &current.angle);

        GXColor emitter2Color;
        emitter2Color.r = tevStr.mColorC0.r;
        emitter2Color.g = tevStr.mColorC0.g;
        emitter2Color.b = tevStr.mColorC0.b;
        emitter2Color.a = tevStr.mColorC0.a;
        
        mEmitters[1] = dComIfGp_particle_setProjection(dPa_name::ID_SCENE_8186, &current.pos, &current.angle, NULL, 0xFF, NULL, current.roomNo, &tevStr.mColorK0, &emitter2Color);

        mEmitters[2] = dComIfGp_particle_setToon(dPa_name::ID_SCENE_A187, &current.pos, &current.angle, NULL, 0xFF, &mSmokeCBs[2]);
        mSmokeCBs[2].setRateOff(0);

        fopAcM_seStartCurrent(this, JA_SE_OBJ_SAGE_GATE_CREAK, 0);
    }
    // After 30 frames, the sound effect for the light beam particles coming from the statue plays.
    else if (mBreakTimer == 30) {
        fopAcM_seStartCurrent(this, JA_SE_OBJ_SAGE_GATE_LIGHT, 0);
    }
    // After 60 frames, the shards that the statue will crumble into begin moving to their initial locations.
    else if (mBreakTimer == 60) {
        m043D = true;

        for (int i = 0; i < 20; i++) {
            mShardPositions[i].setall(0.0f);

            if (i == 0x02 || i == 0x03 || i == 0x06 || i == 0x07 || i == 0x0A || i == 0x0D || i == 0x0E || i == 0x12 || i == 0x13) {
                mShardPositions[i].x += 20.0f;
            }
            else {
                mShardPositions[i].x -= 20.0f;
            }
        }
    }
    // After 160 frames, the particles for the statue shattering spawn.
    else if (mBreakTimer == 160) {
        dComIfGp_getVibration().StartShock(6, -0x21, cXyz(0.0f, 1.0f, 0.0f));

        fopAcM_seStartCurrent(this, JA_SE_OBJ_SAGE_GATE_BREAK, 0);

        mBrokenPos = current.pos;
        mBrokenPos.y += 350.0f;

        mEmitters[3] = dComIfGp_particle_setToon(dPa_name::ID_COMMON_2027, &mBrokenPos, &current.angle, NULL, 0xFF, &mSmokeCBs[3]);
        if (mEmitters[3] != NULL) {
            mEmitters[3]->setVolumeSweep(0.5f);
            mEmitters[3]->setLifeTime(0x2D);
            mEmitters[3]->setRate(50.0f);
            mEmitters[3]->setMaxFrame(1);

            JGeometry::TVec3<f32> vec;

            vec.set(3.0f, 3.0f, 3.0f);
            mEmitters[3]->setGlobalDynamicsScale(vec);

            vec.set(6.0f, 6.0f, 6.0f);
            mEmitters[3]->setGlobalParticleScale(vec);
        }
    }
    // After 255 frames, the cutscene ends.
    else if (mBreakTimer == 255) {
        ret = true;
    }

    /* Main (unbroken) model transparency */
    // From 0 to 60 frames, the main model is 100% visible.
    if (mBreakTimer < 60) {
        mMainMdlAlpha = 0xFF;
    }
    // From 60 to 160 frames, the main model's transparency is lowered to 0.
    else if (mBreakTimer >= 60 && mBreakTimer < 160) {
        f64 mdlAlpha = ((0xA0 - mBreakTimer) * 255.0) / 100.0;

        if (mdlAlpha >= 255.0) {
            mMainMdlAlpha = 0xFF;
        }
        else {
            mMainMdlAlpha = static_cast<u8>(mdlAlpha);
        }
    }
    // After 160 frames, the main model's transparency is 0.
    else {
        mMainMdlAlpha = 0;
    }

    /* Falling shards */
    // After 160 frames, the shards of the broken statue model begin to fall.
    if (mBreakTimer >= 160) {
        int i = 19 - (mBreakTimer - 160);

        if  (i < 0) {
            i = 0;
        }

        for (; i < 20; i++) {
            mShardHeights[i] -= 2.0f;
            mShardPositions[i].y += mShardHeights[i];
        }
    }

    return ret;
}

/* 0000195C-000020E0       .text Execute__Q210daObjMknjD5Act_cFPPA3_A4_f */
BOOL daObjMknjD::Act_c::Execute(Mtx** i_mtx) {
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
    daPy_py_c* partner = static_cast<daPy_py_c*>(dComIfGp_getCb1Player());

    switch (m043F) {
        case 0:
            eventInfo.onCondition(dEvtCnd_CANTALK_e);

            if (eventInfo.checkCommandTalk()) {
                m0500 = 1;
                m043F = 0x0B;
            }
            else if (player != NULL) {
                cXyz partnerDiff = current.pos - player->current.pos;

#if VERSION > VERSION_DEMO
                s16 rotDiff = cM_atan2s(partnerDiff.x, partnerDiff.z) - current.angle.y;
#endif
                f32 absXZ = partnerDiff.absXZ();

                if (absXZ < 800.0f
#if VERSION > VERSION_DEMO
                    && (rotDiff < -0x4000 || rotDiff > 0x4000)
#endif
                ) {
                    if (dComIfGp_checkPlayerStatus1(0, daPyStts1_WIND_WAKER_CONDUCT_e)) {
                        m043F = 1;
                        manage_friend_draw(0);
                    }
                }
            }

            break;
        case 1:
            if (m043E == 1) {
                player->setTactZev(fopAcM_GetID(this), 4, daObjMknjD_EventName[3]);
            }
            else {
                player->setTactZev(fopAcM_GetID(this), 3, daObjMknjD_EventName[2]);
            }

            m043F = 2;
            break;
        case 2:
            if (dComIfGp_evmng_startCheck(mCheckEventIdx) != 0) {
                if (partner != NULL && player != NULL) {
#if VERSION > VERSION_DEMO
                    s16 rotDiff = cM_atan2s(current.pos.x - partner->current.pos.x, current.pos.z - partner->current.pos.z) - current.angle.y;
#endif
                    
                    cXyz diff = player->current.pos - partner->current.pos;
                    f32 absXZ = diff.absXZ();

                    if ((absXZ < 800.0f)
#if VERSION > VERSION_DEMO
                        && ((rotDiff < -0x4000) || (rotDiff > 0x4000))
#endif
                    ) {
                        fopAcM_orderChangeEventId(this, mDemoEventIdx, 0, 0xFFFF);
                        dComIfGs_onEventBit(m0430);

                        s16 procMedli = PROC_NPC_MD;
                        void* judgeResult = fopAcIt_Judge(fpcSch_JudgeForPName, &procMedli);

                        if (judgeResult != NULL) {
                            dComIfGp_event_setTalkPartner(judgeResult);
                        }

                        m043F = 6;
                    }
                    else {
                        fopAcM_orderChangeEventId(this, mErrorEventIdx, 0, 0xFFFF);

                        m043F = 3;
                        manage_friend_draw(1);
                    }
                }
                else {
                    fopAcM_orderChangeEventId(this, mErrorEventIdx, 0, 0xFFFF);

                    m043F = 3;
                    manage_friend_draw(1);
                }
            }
            else {
                if (!dComIfGp_checkPlayerStatus1(0, daPyStts1_WIND_WAKER_CONDUCT_e)) {
                    m043F = 0;
                    manage_friend_draw(1);
                }
            }

            break;
        case 3:
            if (eventInfo.checkCommandDemoAccrpt()) {
                m043F = 4;
            }

            break;
        case 4:
            if (dComIfGp_evmng_endCheck(mErrorEventIdx)) {
                dComIfGp_event_reset();
                m043F = 5;
            }

            break;
        case 5:
            if (!dComIfGp_checkPlayerStatus1(0, daPyStts1_WIND_WAKER_CONDUCT_e)) {
                m043F = 0;
            }

            break;
        case 6:
            if (eventInfo.checkCommandDemoAccrpt()) {
                m043F = 7;

                mDoAud_bgmStop(30);
                mDoAud_taktModeMuteOff();

#if VERSION > VERSION_DEMO
                dComIfGp_att_offAleart();
#endif

                if (m043E == true) {
#if VERSION == VERSION_DEMO
                    m0432 = 0x2B + REG20_S(2);
                    m0434 = 5 + REG20_S(1);
#else
                    m0432 = 0x2B;
                    m0434 = 5;
#endif
                }
                else {
#if VERSION == VERSION_DEMO
                    m0432 = 0x0E + REG20_S(2);
                    m0434 = 5 + REG20_S(1);
#else
                    m0432 = 0x0E;
                    m0434 = 5;
#endif
                }
            }

            break;
        case 7:
            privateCut();

            if (dComIfGp_evmng_endCheck(mDemoEventIdx)) {
#if VERSION > VERSION_DEMO
                dComIfGp_att_revivalAleart();
#endif
                dComIfGp_event_reset();

                fopAcM_delete(this);
            }

            break;
        case 8:
            eventInfo.onCondition(dEvtCnd_CANTALK_e);
            eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);

            if (eventInfo.checkCommandTalk()) {
                if (dComIfGp_event_chkTalkXY()) {
                    m0500 = 0;
                    m043F = 9;

#if VERSION > VERSION_JPN
                    fopAcM_seStart(this, JA_SE_PRE_TAKT, 0);
#endif
                }
                else {
                    m0500 = 1;
                    m043F = 11;
                }
            }

            break;
        case 9:
            m043F = 10;
            break;
        case 10:
            privateCut();

            if (!dComIfGp_event_runCheck()) {
                if (checkItemGet(mGiveItemNo, 1)) {
                    m043F = 0;
                }
                else {
                    m043F = 8;
                }
            }

            break;
        case 11:
            player->onPlayerNoDraw();
            m043F = 12;
            break;
        case 12:
            if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
                player->offPlayerNoDraw();
                dComIfGp_event_reset();

                if (checkItemGet(mGiveItemNo, 1)) {
                    m043F = 0;
                }
                else {
                    m043F = 8;
                }
            }
            break;
    }

    set_mtx();

    *i_mtx = &M_tmp_mtx;

    if (m0432 > 1) {
        m0432--;
    }
    if (m0432 == 1) {
        if (m043E == true) {
            mDoAud_bgmStart(JA_BGM_TAKT_MAKORE);
        }
        else {
            mDoAud_bgmStart(JA_BGM_TAKT_MEDRI);
        }

        m0432 = 0;
    }

    if (m0434 > 1) {
        m0434--;
    }
    if (m0434 == 1) {
        manage_friend_draw(1);
        m0434 = 0;
    }

    return TRUE;
}

/* 000020E0-000022FC       .text setMaterial__10daObjMknjDFP11J3DMaterialUc */
void daObjMknjD::setMaterial(J3DMaterial* i_mat, u8 i_alpha) {
    for (; i_mat != NULL; i_mat = i_mat->getNext()) {
        if (i_alpha == 0) {
            i_mat->getShape()->hide();
        }
        else {
            i_mat->getShape()->show();

            if (i_alpha == 0xFF) {
                i_mat->setMaterialMode(1);

                i_mat->getZMode()->setUpdateEnable(1);
                i_mat->getZMode()->setCompareEnable(1);

                i_mat->getBlend()->setType(GX_BM_NONE);
            }
            else {
                i_mat->setMaterialMode(1);

                i_mat->getZMode()->setUpdateEnable(0);
                i_mat->getZMode()->setCompareEnable(0);

                i_mat->getBlend()->setType(GX_BM_BLEND);
                i_mat->getBlend()->setSrcFactor(GX_BL_SRC_ALPHA);
                i_mat->getBlend()->setDstFactor(GX_BL_INV_SRC_ALPHA);
            }

            i_mat->getTevKColor(3)->mColor.a = i_alpha;
        }
    }
}

/* 000022FC-00002430       .text Draw__Q210daObjMknjD5Act_cFv */
BOOL daObjMknjD::Act_c::Draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mBreakMdl, &tevStr);

    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mMainMdl, &tevStr);

    dComIfGd_setListBG();

    J3DModelData* mdlData = mMainMdl->getModelData();
    u16 jointCount = mdlData->getJointNum();
    for (u16 i = 0; i < jointCount; i++) {
        setMaterial(mdlData->getJointNodePointer(i)->getMesh(), mMainMdlAlpha);
    }

    dComIfGd_setListSky();

    mDoExt_modelUpdateDL(mMainMdl);
    if (m043D == true) {
        mDoExt_modelUpdateDL(mBreakMdl);
    }

    dComIfGd_setList();

    return TRUE;
}

namespace daObjMknjD {
    namespace {
        /* 00002430-00002450       .text Mthd_Create__Q210daObjMknjD27@unnamed@d_a_obj_mknjd_cpp@FPv */
        cPhs_State Mthd_Create(void* i_this) {
            return static_cast<Act_c*>(i_this)->Mthd_Create();
        }

        /* 00002450-00002470       .text Mthd_Delete__Q210daObjMknjD27@unnamed@d_a_obj_mknjd_cpp@FPv */
        BOOL Mthd_Delete(void* i_this) {
            return static_cast<Act_c*>(i_this)->Mthd_Delete();
        }

        /* 00002470-00002490       .text Mthd_Execute__Q210daObjMknjD27@unnamed@d_a_obj_mknjd_cpp@FPv */
        BOOL Mthd_Execute(void* i_this) {
            return static_cast<Act_c*>(i_this)->MoveBGExecute();
        }

        /* 00002490-000024BC       .text Mthd_Draw__Q210daObjMknjD27@unnamed@d_a_obj_mknjd_cpp@FPv */
        BOOL Mthd_Draw(void* i_this) {
            return static_cast<Act_c*>(i_this)->MoveBGDraw();
        }

        /* 000024BC-000024E8       .text Mthd_IsDelete__Q210daObjMknjD27@unnamed@d_a_obj_mknjd_cpp@FPv */
        BOOL Mthd_IsDelete(void* i_this) {
            return static_cast<Act_c*>(i_this)->MoveBGIsDelete();
        }

        static actor_method_class Mthd_Table = {
            (process_method_func)Mthd_Create,
            (process_method_func)Mthd_Delete,
            (process_method_func)Mthd_Execute,
            (process_method_func)Mthd_IsDelete,
            (process_method_func)Mthd_Draw,
        };
    }
}

actor_process_profile_definition g_profile_Obj_MknjD = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_MknjD,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjMknjD::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_MknjD,
    /* Actor SubMtd */ &daObjMknjD::Mthd_Table,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
