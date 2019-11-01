#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

// layout data
// 字段名	字段长度（byte）	取值	备注
// layout len	2		本类型数据长度，包括本字段
// uid len	1		uid长度
// uid data	uid len		uid字符串
// Data len	2		业务数据长度
// Data	Data len		业务数据
// flag	1		(flag & 1) == 1表示存在布局字段；
// alpha	1	[0,255]	视频透明度（布局字段）
// zorder	1	[0,255]	视频层级（布局字段）
// Src width	2		原始视频宽（布局字段）
// Src height	2		原始视频高（布局字段）
// crop left	2		左边裁剪长度（布局字段）
// crop right	2		右边裁剪长度（布局字段）
// crop top	2		上边裁剪长度（布局字段）
// crop bottom	2		下边裁剪长度（布局字段）
// Dest x	2		目标视频x坐标（布局字段）
// Dest y	2		目标视频y坐标（布局字段）
// Dest width	2		目标视频宽度（布局字段）
// Dest height	2		目标视频高度（布局字段）
// DataData len业务数据

typedef unsigned int    u32;
typedef          int    s32;
typedef unsigned short  u16;
typedef          short  s16;
typedef unsigned char   u8;
typedef          char   s8;
typedef unsigned char*  UPSTR; // unsigned pointer string
typedef  u32            BOOL;

#define  FL      (__FUNCTION__, __LINE__)
#define  FALSE   (0)
#define  TRUE    (1)

//==const define begin==========================================================
const u32 KK_H264_SEI_TAG = 0x6;
const u32 KK_H264_SEI_USER_DEF_TAG = 0xC9; // 201
const u32 KK_AUDIO_DSE_TAG = 0x82;

const u8 PTYPE_KK_APP = 1;
const u8 PTYPE_APP_DATA = 2;
const u8 PTYPE_CFG_DATA = 3;
const u8 PTYPE_LAYOUT_DATA = 4;

const static u32 KK_SEI_PAYLOAD_TYPE_YY = 1;

//----error number--------------------------------------------------------------
const static u32 KK_OK = 0;
const static u32 KK_ERR_H264_SEI_DEC_PARA = 20001;
const static u32 KK_ERR_SEI_DEC_DAT_SZ = 20002;

const static u32 KK_ERR_H264_PAYLOAD_DEC_PARA = 20010;
const static u32 KK_ERR_H264_PAYLOAD_DEC_DAT_SZ = 20011;

const static u32 KK_ERR_APP_DAT_PAYLOAD_DEC_PARA = 20016;
const static u32 KK_ERR_APP_DAT_PAYLOAD_DEC_DAT_SZ = 20017;

const static u32 KK_ERR_LAYOUT_PAYLOAD_DEC_PARA = 20020;
const static u32 KK_ERR_LAYOUT_PAYLOAD_DEC_DAT_SZ = 20021;

const static u32 KK_ERR_LAYOUT_DATA_DEC_PARA = 20030;
const static u32 KK_ERR_LAYOUT_DATA_DEC_DAT_SZ = 20031;

const static u32 KK_ERR_CROP_RECT_PARA = 20050;
const static u32 KK_ERR_CROP_RECT_DAT_SZ = 20051;

const static u32 KK_ERR_DST_RECT_PARA = 20053;
const static u32 KK_ERR_DST_RECT_DAT_SZ = 20054;

const static u32 KK_ERR_AUD_DSE_DEC_PARA = 20101;
const static u32 KK_ERR_AUD_DSE_DEC_DAT_SZ = 20102;

const static u32 KK_ERR_AUD_DSE_PAYLOAD_DEC_PARA = 20111;
const static u32 KK_ERR_AUD_DSE_PAYLOAD_DEC_DAT_SZ = 20112;

const static u32 KK_ERR_AUD_DSE_VOL_DATA_DEC_PARA = 20121;
const static u32 KK_ERR_AUD_DSE_VOL_DATA_DEC_DAT_SZ = 20121;

const static u32 KK_ERR_AUD_DSE_VOL_PARA_DEC_PARA = 20151;
const static u32 KK_ERR_AUD_DSE_VOL_PARA_DEC_DAT_SZ = 20152;
const static u32 KK_ERR_AUD_DSE_VOL_PARA_DEC_VOL_LEN = 20153;
const static u32 KK_ERR_AUD_DSE_VOL_PARA_DEC_VOLUME = 20154;

const static u32 KK_ERR_AUD_DSE_VOL_DEC_PARA = 20131;
const static u32 KK_ERR_AUD_DSE_VOL_DEC_VOL_LEN = 20132;
const static u32 KK_ERR_AUD_DSE_VOL_DEC_VOLUME = 20133;

const static u32 KK_ERR_MEM_LAYOUT_PAYLOAD_DATA  = 21001;
const static u32 KK_ERR_MEM_LAYOUT_UID_DATA  = 21002;
const static u32 KK_ERR_MEM_LAYOUT_DATA  = 21003;
//==const define end============================================================
//==============================================================================

//==struct common define begin==================================================
typedef struct tag_data_size{
    u8* pdata;
    u32 size;
}t_data_size;

//==struct video sei define begin===============================================
typedef struct tag_kk_rect{
    u16  x;
    u16  y;
    u16  width;
    u16  height;
}t_kk_rect;

typedef struct tag_kk_layout_data{
    u16 length;
    u8  uid_len;
    u8* puid_data;   // 1 byte, 1
    u16 data_len;
    u8* pdata;
    u8  flag;
    u8  alpha;
    u8  zorder;
    u16 src_width;
    u16 src_height;
    t_kk_rect tcrop;
    t_kk_rect tdest;
    struct tag_kk_layout_data *pnext;
}t_kk_layout_data;

typedef struct tag_kk_h264_sei{
    u32 nal_size;      // 4 bytes, 0x00000001
    u8  unit_type;;    // 1 byte, 78
    u8  payload_type;  // 1 byte, 201
    u32 payload_size;  // payload_size/255+1
    u8* ppayload;
    u8  tailing_bit;
}t_kk_h264_sei;

typedef struct tag_kk_h265_sei{
    u32 nal_size;      // 4 bytes, 0x00000001
    u8  unit_type;;    // 1 byte, 78
    u8  temporal_id;   // 1 byte, 1
    u8  payload_type;  // 1 byte, 201
    u32 payload_size;  // payload_size/255+1
    u8* ppayload;
    u8  tailing_bit;
}t_kk_h265_sei;

typedef struct tag_kk_sei_payload{
    u16  length;
    u8   payload_type;
    u8*  payload;
}t_kk_sei_payload;

typedef struct tag_kk_app_data_payload{
    u16   data_count;
    u8*   pdata;
    struct tag_kk_app_data_payload* pnext;
}t_kk_app_data_payload;

typedef struct tag_kk_app_data{
    u32   data;
    t_kk_app_data_payload* papp_list;
}t_kk_app_data;

typedef struct tag_kk_layout_app_payload{
    u8   data_cnt;
    t_kk_layout_data* playout_data_list;
}t_kk_layout_app_payload;

typedef struct tag_kk_cfg_data_payload{
    u8   flag;
}t_kk_cfg_data_payload;
//==struct video sei define end=================================================

//==struct audio dse define begin===============================================
typedef struct tag_kk_aud_dse_info{
    u8  id;
    u8  count;
    u8  esc_count;
    u8* payload;
}t_kk_aud_dse_info;

typedef struct tag_kk_dse_payload{
    u16   length;
    u8    type;
    u8*   data;
}t_kk_dse_payload;

typedef struct tag_kk_volume_data{
    u8   volume_cnt;
    struct tag_kk_volume_data* papp_list;
}t_kk_volume_data;

typedef struct tag_kk_volume_para{
    u16 length;
    u8  uid_len;
    u8* puid_str; // memory
    u8  volume;
    struct tag_kk_volume_para *pnext;
}t_kk_volume_para;
//==struct audio dse define end=================================================

//==struct common define begin==================================================
typedef struct tag_kk_video_sei{
    t_kk_h264_sei t_sei_info;
    t_kk_sei_payload t_sei_payload;
    t_kk_app_data_payload t_sei_app_data_payload;
    t_kk_layout_app_payload t_layout_data;
    t_kk_cfg_data_payload tcfg_data;
}t_kk_video_sei;

typedef struct tag_kk_audio_dse{
    t_kk_aud_dse_info t_aud_dse_info;
    t_kk_dse_payload t_aud_dse_payload;
    t_kk_volume_data t_aud_vol_data;
    t_kk_volume_para t_aud_vol_para;
}t_kk_audio_dse;

typedef struct tag_kk_sei_dse{
    t_kk_video_sei t_vid_sei;
    t_kk_audio_dse t_aud_dse;
}t_kk_sei_dse;
//==struct define end===========================================================
//==============================================================================

//====function define begin=====================================================
#define BSWAP_16(x) \
    (u16)((((u16)(x) & 0x00ff) << 8) | \
          (((u16)(x) & 0xff00) >> 8) \
        )  

#define BSWAP_32(x) \
    (u32)( (((u32)(x) & 0xff000000) >> 24) | \
            (((u32)(x) & 0x00ff0000) >> 8) | \
            (((u32)(x) & 0x0000ff00) << 8) | \
            (((u32)(x) & 0x000000ff) << 24) \
            )  

#define  KK_MOVE_BYTE(ret_dat, psrc)\
    {                              \
        ret_dat = *(psrc->pdata);  \
        psrc->pdata++;             \
    }

#define  KK_MOVE_WORD(ret_dat, tmp_dat, psrc)\
    {                                  \
        tmp_dat = *(u16 *)psrc->pdata; \
        ret_dat = BSWAP_16(tmp_dat);   \
        psrc->pdata += 2;              \
    }

#define  KK_MOVE_BUF(pdst, data_len, psrc, ret, err_no)\
{                                                           \
        pdst = (u8 *)malloc(data_len + 1);                  \
        if (NULL == pdst)                                   \
        {                                                   \
            printf("[%s:%d]-memory-fail:%d\n", FL, err_no); \
            ret = err_no;                                   \
        }                                                   \
        else                                                \
        {                                                   \
            memset(pdst, 0, data_len + 1);                  \
            memcpy(pdst, psrc->pdata, data_len);            \
            psrc->pdata += data_len;                        \
        }                                                   \
}

#define  KK_FUNC_PARA_RET(psei, pds, err_no1, err_no2)\
    if ( (NULL == psei) || (NULL == pds) )\
    {\
        printf("E-[%s:%d]-parameter, psei:%p, pdat_sz:%p\n", FL, psei, pds);\
        return err_no1;\
    }\
    if ( (NULL == pds->pdata) || (pds->size < 1) )\
    {\
        printf("E-[%s:%d]-parameter, data:%p, size:%d\n", FL, pds->pdata, pds->size);\
        return err_no2;\
    }
//====function define end=======================================================
//==============================================================================

static t_kk_sei_dse g_sei_dse;

static void kk_log_layout_data(t_kk_layout_data* pdat)
{
    if (NULL == pdat)
    {
        return;
    }

    printf("length....:%d[%x]\n", pdat->length, pdat->length);
    printf("uid_len...:%d[%x]\n", pdat->uid_len, pdat->uid_len);
    printf("puid_data.:%s\n",     pdat->puid_data);
    printf("data_len..:%d[%x]\n", pdat->data_len, pdat->data_len);
    printf("pdata.....:%s\n",     pdat->pdata);
    printf("flag......:%d[%x]\n", pdat->flag, pdat->flag);
    printf("alpha.....:%d[%x]\n", pdat->alpha, pdat->alpha);
    printf("zorder....:%d[%x]\n", pdat->zorder, pdat->zorder);
    printf("src_width.:%d[%x]\n", pdat->src_width, pdat->src_width);
    printf("src_height:%d[%x]\n", pdat->src_height, pdat->src_height);

    printf("tcrop[x,y,w,h]:[%d-%d-%d-%d][%x-%x-%x-%x]\n", 
        pdat->tcrop.x, pdat->tcrop.y, pdat->tcrop.width, pdat->tcrop.height,
        pdat->tcrop.x, pdat->tcrop.y, pdat->tcrop.width, pdat->tcrop.height);

    printf("tdest[x,y,w,h]:[%d-%d-%d-%d][%x-%x-%x-%x]\n", 
        pdat->tdest.x, pdat->tdest.y, pdat->tdest.width, pdat->tdest.height,
        pdat->tdest.x, pdat->tdest.y, pdat->tdest.width, pdat->tdest.height);
}


static u32 app_data_item_dec(t_kk_video_sei *psei, t_data_size* pds)
{
    return KK_OK;
}

static u32 app_data_payload_dec(t_kk_video_sei *psei, t_data_size* pds)
{
    KK_FUNC_PARA_RET(psei, pds, KK_ERR_APP_DAT_PAYLOAD_DEC_PARA, KK_ERR_APP_DAT_PAYLOAD_DEC_DAT_SZ);
    u8 data_cnt = 0;
    KK_MOVE_BYTE(data_cnt, pds);
    psei->t_sei_app_data_payload.data_count = data_cnt;
    u32 id;
    u32 iret = KK_OK;
    for (id = 0; id < data_cnt; id++)
    {
        iret = app_data_item_dec(psei, pds);
        if (KK_OK != iret)
        {
            return iret;
        }
    }
    return KK_OK;
}

static u32 rect_dec(t_kk_rect* prect, t_data_size* pds, u32 er_no1, u32 er_no2)
{
    KK_FUNC_PARA_RET(prect, pds, er_no1, er_no2);
    u32 tmp_len = 0;
    KK_MOVE_WORD(prect->x, tmp_len, pds);
    KK_MOVE_WORD(prect->y, tmp_len, pds);
    KK_MOVE_WORD(prect->width, tmp_len, pds);
    KK_MOVE_WORD(prect->height, tmp_len, pds);
    return KK_OK;
}

static void kk_layout_data_list_add(t_kk_video_sei *psei, t_kk_layout_data* playout_dat)
{
}

static u32 layout_data_dec(t_kk_video_sei *psei, t_data_size* pds)
{
    KK_FUNC_PARA_RET(psei, pds, KK_ERR_LAYOUT_DATA_DEC_PARA, KK_ERR_LAYOUT_DATA_DEC_DAT_SZ);

    u16 tmp_len = sizeof(t_kk_layout_data) + 1;
    u16 data_len = 0;
    u32 iret = KK_OK;
    t_kk_layout_data* playout_dat = (t_kk_layout_data *)malloc(tmp_len);
    memset(playout_dat, 0, tmp_len);

    KK_MOVE_WORD(playout_dat->length, tmp_len, pds);

    KK_MOVE_BYTE(playout_dat->uid_len, pds);
    KK_MOVE_BUF(playout_dat->puid_data, playout_dat->uid_len, pds, iret, KK_ERR_MEM_LAYOUT_UID_DATA);
    if (KK_OK != iret)
    {
        return iret;
    }

    KK_MOVE_WORD(playout_dat->data_len, tmp_len, pds);
    KK_MOVE_BUF(playout_dat->pdata, playout_dat->data_len, pds, iret, KK_ERR_MEM_LAYOUT_DATA);
    if (KK_OK != iret)
    {
        return iret;
    }

    KK_MOVE_BYTE(playout_dat->flag, pds);
    KK_MOVE_BYTE(playout_dat->alpha, pds);
    KK_MOVE_BYTE(playout_dat->zorder, pds);
    KK_MOVE_WORD(playout_dat->src_width, tmp_len, pds);
    KK_MOVE_WORD(playout_dat->src_height, tmp_len, pds);

    iret = rect_dec(&(playout_dat->tcrop), pds, KK_ERR_CROP_RECT_PARA, KK_ERR_CROP_RECT_DAT_SZ);
    if (KK_OK != iret)
    {
        return iret;
    }
    iret = rect_dec(&(playout_dat->tdest), pds, KK_ERR_DST_RECT_PARA, KK_ERR_DST_RECT_DAT_SZ);
    if (KK_OK != iret)
    {
        return iret;
    }

    kk_log_layout_data(playout_dat);

    kk_layout_data_list_add(psei, playout_dat);

    return KK_OK;
}

static u32 layout_data_payload_dec(t_kk_video_sei *psei, t_data_size* pds)
{
    KK_FUNC_PARA_RET(psei, pds, KK_ERR_LAYOUT_PAYLOAD_DEC_PARA, KK_ERR_LAYOUT_PAYLOAD_DEC_DAT_SZ);
    u32 data_cnt = 0;
    KK_MOVE_BYTE(data_cnt, pds);
    psei->t_layout_data.data_cnt = data_cnt;
    printf("sei layout data cnt:%d[%x]\n", data_cnt, data_cnt);
    u32 id;
    u32 iret = KK_OK;
    for (id = 0; id < data_cnt; id++)
    {
        iret = layout_data_dec(psei, pds);
        if (KK_OK != iret)
        {
            return iret;
        }
    }
    return KK_OK;
}

// 1:kk app data
// 2:app data
// 3:config data
// 4:layout data
static u32 h264_payload_dec(t_kk_video_sei *psei, t_data_size* pds)
{
    KK_FUNC_PARA_RET(psei, pds, KK_ERR_H264_PAYLOAD_DEC_PARA, KK_ERR_H264_PAYLOAD_DEC_DAT_SZ);
    u32 iret = KK_OK;
    u16 tmp = 0;
    u16 wlen = 0;
    u8 bytype = 0; 
    KK_MOVE_WORD(wlen, tmp, pds);
    psei->t_sei_payload.length = wlen;
    KK_MOVE_BYTE(bytype, pds);
    psei->t_sei_payload.payload_type = bytype;
    printf("sei payload len:%d[%x], type:%d[%x]\n", wlen, wlen, bytype, bytype);

    switch (bytype)
    {
        case PTYPE_KK_APP:
        break;
        case PTYPE_APP_DATA:
            app_data_payload_dec(psei, pds);
        break;
        case PTYPE_CFG_DATA:
            KK_MOVE_BYTE(psei->tcfg_data.flag, pds);
        break;
        case PTYPE_LAYOUT_DATA:
            iret = layout_data_payload_dec(psei, pds);
        break;
        default:
            printf("unnkow type:%d\n", bytype);
        break;
    }

    return iret;
}

u32 h264_sei_dec(t_kk_video_sei *psei, t_data_size* pds)
{
    KK_FUNC_PARA_RET(psei, pds, KK_ERR_H264_SEI_DEC_PARA, KK_ERR_SEI_DEC_DAT_SZ);
    u32 ret = KK_OK;

    u8 nal_type = *(pds->pdata);
    u8 payload_type = *(pds->pdata + 1);
    pds->pdata += 2;
    pds->size -= 2;
    switch (nal_type)
    {
        case KK_H264_SEI_TAG:
            printf("nal-type:is h264:%d[%x]\n", nal_type, nal_type);
            if (KK_H264_SEI_USER_DEF_TAG == payload_type)
            {
                printf("nal-type:is user-define-payload:%d[%x]\n", payload_type, payload_type);
                u32 payload_sz = 0;
                while (payload_sz % 0xFF == 0) {
                    payload_sz += *(pds->pdata);
                    pds->pdata++;
                    pds->size--;
                    printf("payload_size-in[%d][%x]\n", payload_sz, payload_sz);
                }
                printf("payload_size:[%d][%x]\n", payload_sz, payload_sz);
                ret = h264_payload_dec(psei, pds);
                // pds->pdata += payload_sz;
                pds->size -= payload_sz;
            }
        break;
        case 78:
            printf("nal-type:is h265\n");
            if (KK_H264_SEI_USER_DEF_TAG == payload_type)
            {
                printf("nal-type:is user-define-payload\n");
            }
        break;
        default:
            pds->pdata -= 2;
            pds->size += 2;
            printf("unknow-nal-type:%d\n", nal_type);
        break;
    }

    return ret;
}

// ===================================================================
static u32 aud_dse_vol_para_dec(t_kk_volume_para *pvol_para, t_data_size* pds)
{
    KK_FUNC_PARA_RET(pvol_para, pds, KK_ERR_AUD_DSE_VOL_PARA_DEC_PARA, KK_ERR_AUD_DSE_VOL_PARA_DEC_DAT_SZ);
    u32 iret = KK_OK;
    u16 tmp = 0;
    u16 vol_len = 0;
    KK_MOVE_WORD(vol_len, tmp, pds);
    if ( (vol_len < 2) || (vol_len > 500) )
    {
        printf("[%s:%d][E]volume length:%d, must in[2-500]\n", FL, vol_len);
        return KK_ERR_AUD_DSE_VOL_PARA_DEC_VOL_LEN;
    }
    pvol_para->length = vol_len;
    KK_MOVE_BYTE(tmp, pds);
    pvol_para->uid_len = tmp;
    KK_MOVE_BUF(pvol_para->puid_str, tmp, pds, iret, 123444);
    if (KK_OK != iret)
    {
        return iret;
    }
    KK_MOVE_BYTE(tmp, pds);
    if (tmp > 100)
    {
        printf("[%s:%d][E]volume:%d, must in[0-100]\n", FL, tmp);
        return KK_ERR_AUD_DSE_VOL_PARA_DEC_VOLUME;
    }
    pvol_para->volume = tmp;
    return iret;
}

static u32 aud_dse_vol_data_dec(t_kk_audio_dse *pdse, t_data_size* pds)
{
    KK_FUNC_PARA_RET(pdse, pds, KK_ERR_AUD_DSE_VOL_DATA_DEC_PARA, KK_ERR_AUD_DSE_VOL_DATA_DEC_DAT_SZ);
    u32 ret = KK_OK;
    u32 data_cnt = 0;
    KK_MOVE_BYTE(data_cnt, pds);
    pdse->t_aud_vol_data.volume_cnt = data_cnt;
    u32 id;
    for (id = 0; id < data_cnt; id++)
    {
        ret = aud_dse_vol_para_dec(&(pdse->t_aud_vol_para), pds);
        if (KK_OK != ret)
        {
            return ret;
        }
    }
    
    return ret;
}

static u32 aud_dse_payload_dec(t_kk_audio_dse *pdse, t_data_size* pds)
{
    KK_FUNC_PARA_RET(pdse, pds, KK_ERR_AUD_DSE_PAYLOAD_DEC_PARA, KK_ERR_AUD_DSE_PAYLOAD_DEC_DAT_SZ);
    u32 ret = KK_OK;
    u16 tmp_len = 0;
    KK_MOVE_WORD(pdse->t_aud_dse_payload.length, tmp_len, pds);
    KK_MOVE_BYTE(pdse->t_aud_dse_payload.type, pds);
    aud_dse_vol_data_dec(pdse, pds);
    return ret;
}

u32 aud_dse_dec(t_kk_audio_dse *pdse, t_data_size* pds)
{
    KK_FUNC_PARA_RET(pdse, pds, KK_ERR_AUD_DSE_DEC_PARA, KK_ERR_AUD_DSE_DEC_DAT_SZ);
    u32 ret = KK_OK;
    u8 aud_dse_id = 0;
    KK_MOVE_BYTE(aud_dse_id, pds);
    pdse->t_aud_dse_info.id = aud_dse_id;
    if (KK_AUDIO_DSE_TAG == aud_dse_id)
    {
        KK_MOVE_BYTE(pdse->t_aud_dse_info.count, pds);
        KK_MOVE_BYTE(pdse->t_aud_dse_info.esc_count, pds);
        u32 pay_load_size = pdse->t_aud_dse_info.count
            +pdse->t_aud_dse_info.esc_count;
        aud_dse_payload_dec(pdse, pds);
    }
    
    return ret;
}


#define FILE_SRC  "C:\\Users\\isuke\\10078-1.h264"
u32 is_aud_dse_tag = 0;
u32 tst_h264_se(void)
{
    FILE *fp_src = fopen(FILE_SRC, "rb");
    if (NULL == fp_src)
    {
        printf("open file:%s fail\n", FILE_SRC);
        return 20500;
    }

    t_kk_video_sei *pvid_sei = &g_sei_dse.t_vid_sei;
    t_kk_audio_dse *paud_dse = &g_sei_dse.t_aud_dse;
    
    printf("sizeof(g_sei_dse):%d\n", sizeof(g_sei_dse));
    memset(&g_sei_dse, 0, sizeof(g_sei_dse));

    fseek(fp_src, 0L, SEEK_END);
    u32 fsize = ftell(fp_src);
    printf("file:%s, size:%d\n", FILE_SRC, fsize);
    fseek(fp_src, 0L, SEEK_SET);
    u8* pbybuf = (u8*)malloc(fsize);
    if (NULL == pbybuf)
    {
        printf("memory for file size:%d buffer fail\n", fsize);
        return 20510;
    }
    fread(pbybuf, fsize, 1, fp_src);
    u32 icount = 0;
    t_data_size tdat_sz = {pbybuf, fsize};
    while (tdat_sz.size > 0)
    {
        u8* tmp_buf = tdat_sz.pdata;
        printf("tmp_buf:0x%p\n", tmp_buf);
        if ( (0 == *tmp_buf) && (0 == *(tmp_buf+1)) && (0 == *(tmp_buf+2)) && (1 == *(tmp_buf+3)) )
        {
            icount++;
            printf("find video-h264-tag:%d\n", icount);
            tdat_sz.pdata += 4;
            tdat_sz.size -= 4;
            h264_sei_dec(pvid_sei, &tdat_sz);
        }
        else if (is_aud_dse_tag)
        {
            printf("find audio-dse-tag:%d\n", icount);
            aud_dse_dec(paud_dse, &tdat_sz);
        }
        else
        {
            printf("dat:%x\n", *tdat_sz.pdata);
            tdat_sz.pdata++;
            tdat_sz.size--;
        }
    }
    
    fclose(fp_src);
    return 0;
}

int main(void)
{
    printf("--------main-bgn================");
    tst_h264_se();
    printf("--------func-end================");
    system("pause");
    printf("--------main-ext================");
    return 0;
}

