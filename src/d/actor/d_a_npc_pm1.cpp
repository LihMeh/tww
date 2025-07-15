/**
 * d_a_npc_pm1.cpp
 * NPC - Maggie (poor)
 */

#include "d/actor/d_a_npc_pm1.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"

daNpc_Pm1_HIO_c l_HIO;

/* 000000EC-00000144       .text __ct__15daNpc_Pm1_HIO_cFv */
daNpc_Pm1_HIO_c::daNpc_Pm1_HIO_c() {
    /* Nonmatching */
}

/* 00000144-00000300       .text nodeCallBack_Pm__FP7J3DNodei */
static BOOL nodeCallBack_Pm(J3DNode*, int) {
    /* Nonmatching */
}

/* 0000033C-0000044C       .text createInit__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::createInit() {
    mEventCut.setActorInfo2("Pm1", this);
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_TALKFLAG_LOOK_e;
    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xAB;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xA9;
    gravity = -4.0f;
}

/* 0000044C-0000055C       .text setMtx__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::setMtx() {
    /* Nonmatching */
}

/* 0000055C-00000664       .text anmResID__11daNpc_Pm1_cFiPiPi */
void daNpc_Pm1_c::anmResID(int, int*, int*) {
    /* Nonmatching */
}

/* 00000664-00000700       .text BtpNum2ResID__11daNpc_Pm1_cFiPi */
void daNpc_Pm1_c::BtpNum2ResID(int, int*) {
    /* Nonmatching */
}

/* 00000700-00000744       .text setAnm_tex__11daNpc_Pm1_cFSc */
void daNpc_Pm1_c::setAnm_tex(signed char) {
    /* Nonmatching */
}

/* 00000744-00000860       .text init_btp__11daNpc_Pm1_cFbi */
void daNpc_Pm1_c::init_btp(bool, int) {
    /* Nonmatching */
}

/* 00000860-000008A4       .text initTexPatternAnm__11daNpc_Pm1_cFb */
void daNpc_Pm1_c::initTexPatternAnm(bool) {
    /* Nonmatching */
}

/* 000008A4-00000968       .text playTexPatternAnm__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::playTexPatternAnm() {
    /* Nonmatching */
}

/* 00000968-00000A34       .text setAnm_anm__11daNpc_Pm1_cFPQ211daNpc_Pm1_c9anm_prm_c */
void daNpc_Pm1_c::setAnm_anm(daNpc_Pm1_c::anm_prm_c*) {
    /* Nonmatching */
}

/* 00000A34-00000A9C       .text setAnm__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::setAnm() {
    /* Nonmatching */
}

/* 00000A9C-00000AA8       .text chngAnmTag__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::chngAnmTag() {
    /* Nonmatching */
}

/* 00000AA8-00000AB4       .text ctrlAnmTag__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::ctrlAnmTag() {
    /* Nonmatching */
}

/* 00000AB4-00000AF4       .text chngAnmAtr__11daNpc_Pm1_cFUc */
void daNpc_Pm1_c::chngAnmAtr(unsigned char) {
    /* Nonmatching */
}

/* 00000AF4-00000AF8       .text ctrlAnmAtr__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::ctrlAnmAtr() {
}

/* 00000AF8-00000B60       .text setAnm_ATR__11daNpc_Pm1_cFi */
void daNpc_Pm1_c::setAnm_ATR(int) {
    /* Nonmatching */
}

/* 00000B60-00000C1C       .text anmAtr__11daNpc_Pm1_cFUs */
void daNpc_Pm1_c::anmAtr(unsigned short) {
    /* Nonmatching */
}

/* 00000C1C-00000C74       .text setStt__11daNpc_Pm1_cFSc */
void daNpc_Pm1_c::setStt(s8 i_new_value) {
    s8 old_value = field_0x7D0;
    field_0x7C5 = i_new_value;
    if (field_0x7D0 == 0x2) {
        field_0x7D2 = 0x1;
        field_0x7CB = 0xFF;
        field_0x7D1 = old_value;
    } else {
        setAnm();
    }
}

/* 00000C74-00000C7C       .text next_msgStatus__11daNpc_Pm1_cFPUl */
u16 daNpc_Pm1_c::next_msgStatus(u32* pMsgNo) {
    return fopMsgStts_MSG_ENDS_e;
}

/* 00000C7C-00000C84       .text getMsg__11daNpc_Pm1_cFv */
u32 daNpc_Pm1_c::getMsg() {
    return 0;
}

/* 00000C84-00000CD4       .text eventOrder__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::eventOrder() {
    /* Nonmatching */
}

/* 00000CD4-00000D14       .text checkOrder__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::checkOrder() {
    u16 command = eventInfo.getCommand();
    if (command == dEvtCmd_INDEMO_e) {
        return;
    }
    if (command != dEvtCmd_INTALK_e) {
        return;
    }
    if (field_0x7CF != 1 && field_0x7CF != 2) {
        return;
    }
    field_0x7CF = 0;
    field_0x7C5 = 1;
}

/* 00000D14-00000EA4       .text lookBack__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::lookBack() {
    /* Nonmatching */
}

/* 00000EA4-00000F24       .text chkAttention__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::chkAttention() {
    /* Nonmatching */
}

/* 00000F24-00000F88       .text setAttention__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::setAttention() {
    float y = current.pos.y + l_HIO.field_0x20;
    attention_info.position.x = current.pos.x;
    attention_info.position.y = y;
    attention_info.position.z = current.pos.z;

    if (field_0x7BC == 0 && field_0x7C0 == 0) {
        return;
    }

    eyePos = field_0x78C;
    field_0x7BC = 0;
}

/* 00000F88-00000FB4       .text decideType__11daNpc_Pm1_cFi */
void daNpc_Pm1_c::decideType(int) {
    /* Nonmatching */
}

/* 00000FB4-0000103C       .text event_actionInit__11daNpc_Pm1_cFi */
void daNpc_Pm1_c::event_actionInit(int) {
    /* Nonmatching */
}

/* 0000103C-0000104C       .text event_action__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::event_action() {
    /* Nonmatching */
}

/* 0000104C-00001154       .text privateCut__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::privateCut() {
    /* Nonmatching */
}

/* 00001154-00001174       .text endEvent__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::endEvent() {
    /* Nonmatching */
}

/* 00001174-000011D4       .text event_proc__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::event_proc() {
    if (!mEventCut.cutProc()) {
        privateCut();
    }
    lookBack();
    shape_angle = current.angle;
}

/* 000011D4-00001280       .text set_action__11daNpc_Pm1_cFM11daNpc_Pm1_cFPCvPvPv_iPv */
void daNpc_Pm1_c::set_action(int (daNpc_Pm1_c::*)(void*), void*) {
    /* Nonmatching */
}

/* 00001280-00001378       .text wait01__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::wait01() {
    /* Nonmatching */
}

/* 00001378-00001408       .text talk01__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::talk01() {
    /* Nonmatching */
}

/* 00001408-000014BC       .text wait_action1__11daNpc_Pm1_cFPv */
void daNpc_Pm1_c::wait_action1(void*) {
    /* Nonmatching */
}

/* 000014BC-00001558       .text demo__11daNpc_Pm1_cFv */
void daNpc_Pm1_c::demo() {
    /* Nonmatching */
}

/* 00001558-000016BC       .text _draw__11daNpc_Pm1_cFv */
BOOL daNpc_Pm1_c::_draw() {
    /* Nonmatching */
}

/* 000016BC-00001818       .text _execute__11daNpc_Pm1_cFv */
BOOL daNpc_Pm1_c::_execute() {
    /* Nonmatching */
}

/* 00001818-0000189C       .text _delete__11daNpc_Pm1_cFv */
BOOL daNpc_Pm1_c::_delete() {
    dComIfG_resDelete(&mPhs, "Pm");
    if (mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }

    // TODO: match the HIO part

    return TRUE;
}

/* 0000189C-000018BC       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Pm1_c*>(i_this)->CreateHeap();
}

/* 000018BC-00001A2C       .text _create__11daNpc_Pm1_cFv */
cPhs_State daNpc_Pm1_c::_create() {
    /* Nonmatching */
}

/* 00001E5C-00002168       .text CreateHeap__11daNpc_Pm1_cFv */
BOOL daNpc_Pm1_c::CreateHeap() {
    return TRUE;
}

/* 00002168-00002188       .text daNpc_Pm1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Pm1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Pm1_c*)i_this)->_create();
}

/* 00002188-000021A8       .text daNpc_Pm1_Delete__FP11daNpc_Pm1_c */
static BOOL daNpc_Pm1_Delete(daNpc_Pm1_c* i_this) {
    return ((daNpc_Pm1_c*)i_this)->_delete();
}

/* 000021A8-000021C8       .text daNpc_Pm1_Execute__FP11daNpc_Pm1_c */
static BOOL daNpc_Pm1_Execute(daNpc_Pm1_c* i_this) {
    return ((daNpc_Pm1_c*)i_this)->_execute();
}

/* 000021C8-000021E8       .text daNpc_Pm1_Draw__FP11daNpc_Pm1_c */
static BOOL daNpc_Pm1_Draw(daNpc_Pm1_c* i_this) {
    return ((daNpc_Pm1_c*)i_this)->_draw();
}

/* 000021E8-000021F0       .text daNpc_Pm1_IsDelete__FP11daNpc_Pm1_c */
static BOOL daNpc_Pm1_IsDelete(daNpc_Pm1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Pm1_Method = {
    (process_method_func)daNpc_Pm1_Create,
    (process_method_func)daNpc_Pm1_Delete,
    (process_method_func)daNpc_Pm1_Execute,
    (process_method_func)daNpc_Pm1_IsDelete,
    (process_method_func)daNpc_Pm1_Draw,
};

actor_process_profile_definition g_profile_NPC_PM1 = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_NPC_PM1,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Pm1_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_NPC_PM1,
    /* Actor SubMtd */ &l_daNpc_Pm1_Method,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
