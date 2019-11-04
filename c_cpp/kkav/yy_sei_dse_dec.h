#ifndef __YY_SEI_DSE_DEC_H__
#define __YY_SEI_DSE_DEC_H__

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
#define  KK_H264_SEI_TAG  0x6
#define  KK_H264_SEI_USER_DEF_TAG  0xC9 // 201
#define  KK_AUDIO_DSE_TAG  0x82

typedef enum{

    ePTYPE_KK_APP = 1,
    ePTYPE_APP_DATA,
    ePTYPE_CFG_DATA,
    ePTYPE_LAYOUT_DATA
}emKK_SEI_TYPE;


#define   KK_SEI_PAYLOAD_TYPE_YY  1

//----error number--------------------------------------------------------------
#define   KK_OK  0
#define   KK_ERR_H264_SEI_DEC_PARA  20001
#define   KK_ERR_SEI_DEC_DAT_SZ  20002

#define   KK_ERR_H264_PAYLOAD_DEC_PARA  20010
#define   KK_ERR_H264_PAYLOAD_DEC_DAT_SZ  20011

#define   KK_ERR_APP_DAT_PAYLOAD_DEC_PARA  20016
#define   KK_ERR_APP_DAT_PAYLOAD_DEC_DAT_SZ  20017

#define   KK_ERR_LAYOUT_PAYLOAD_DEC_PARA  20020
#define   KK_ERR_LAYOUT_PAYLOAD_DEC_DAT_SZ  20021

#define   KK_ERR_LAYOUT_DATA_DEC_PARA  20030
#define   KK_ERR_LAYOUT_DATA_DEC_DAT_SZ  20031

#define   KK_ERR_CROP_RECT_PARA  20050
#define   KK_ERR_CROP_RECT_DAT_SZ  20051

#define   KK_ERR_DST_RECT_PARA  20053
#define   KK_ERR_DST_RECT_DAT_SZ  20054

#define   KK_ERR_AUD_DSE_DEC_PARA  20101
#define   KK_ERR_AUD_DSE_DEC_DAT_SZ  20102

#define   KK_ERR_AUD_DSE_PAYLOAD_DEC_PARA  20111
#define   KK_ERR_AUD_DSE_PAYLOAD_DEC_DAT_SZ  20112

#define   KK_ERR_AUD_DSE_VOL_DATA_DEC_PARA  20121
#define   KK_ERR_AUD_DSE_VOL_DATA_DEC_DAT_SZ  20121

#define   KK_ERR_AUD_DSE_VOL_PARA_DEC_PARA  20151
#define   KK_ERR_AUD_DSE_VOL_PARA_DEC_DAT_SZ  20152
#define   KK_ERR_AUD_DSE_VOL_PARA_DEC_VOL_LEN  20153
#define   KK_ERR_AUD_DSE_VOL_PARA_DEC_VOLUME  20154

#define   KK_ERR_AUD_DSE_VOL_DEC_PARA  20131
#define   KK_ERR_AUD_DSE_VOL_DEC_VOL_LEN  20132
#define   KK_ERR_AUD_DSE_VOL_DEC_VOLUME  20133

#define   KK_ERR_MEM_LAYOUT_PAYLOAD_DATA   21001
#define   KK_ERR_MEM_LAYOUT_UID_DATA   21002
#define   KK_ERR_MEM_LAYOUT_DATA   21003
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

u32 kk_h264_sei_dec(t_kk_video_sei *psei, t_data_size* pds);
u32 kk_aud_dse_dec(t_kk_audio_dse *pdse, t_data_size* pds);

#endif

