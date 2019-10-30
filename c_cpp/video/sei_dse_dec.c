
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

#include <stdio.h>

#define  FL      (__FUNCTION__, __LINE__)
#define  FALSE   (0)
#define  TRUE    (1)


const int KK_AUDIO_DSE_TAG = 0x82;

typedef  int           BOOL;


typedef unsigned int   u32;
typedef          int   s32;
typedef unsigned short u16;
typedef          short s16;
typedef unsigned char  u8;
typedef          char  s8;
typedef unsigned char* UPSTR; // unsigned pointer string

// =====<<h264 sei===============
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
    u16 uiddata_len;
    u16 data_data_len;
    u8* pdata;
    u8  flag;
    u8  alpha;
    u8  zorder;
    u16 src_width;
    u16 src_height;
    t_kk_rect tcrop;
    t_kk_rect tdest;

    // UPSTR payloads;

    t_kk_layout_data *pnext;
}t_kk_layout_data;


typedef struct tag_kk_h264_sei{
    u32 nal_size;      // 4 bytes, 0x00000001
    u8  unit_type;;    // 1 byte, 78
    u8  payload_type;  // 1 byte, 201
    u32 payload_size;  // payload_size/255+1
    u8* ppayload;
    u8  tailing_bit;
}t_kk_h264_sei;

// h265_sei
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


// =====>>h264 sei===============

// =====<<audio dse===============

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
    t_kk_volume_para* papp_list;
}t_kk_volume_data;

typedef struct tag_kk_volume_para{
    u16 length;
    u8  uid_len;
    u8* puid_str; // malloc
    u8  volume;
    t_kk_volume_para *pnext;
}t_kk_volume_para;


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

// =====>>audio dse===============

// struct FrameListData {
//     void *buf;                       /**< compressed data buffer */
//     size_t sz;                       /**< length of compressed data */
//     void *buf_alpha;
//     size_t sz_alpha;
//     int64_t pts;                     /**< time stamp to show frame
//                                           (in timebase units) */
//     unsigned long duration;          /**< duration to show frame
//                                           (in timebase units) */
//     uint32_t flags;                  /**< flags for this frame */
//     uint64_t sse[4];
//     int have_sse;                    /**< true if we have pending sse[] */
//     uint64_t frame_number;
//     struct FrameListData *next;
// };

// typedef struct tag_kk_h264_sei{
//     s32 sei_payload_size;  // payload_size/255+1
//     UPSTR payloads;
// }t_kk_h264_sei;


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

// int h264_sei_dec(void *pv_data, int n_size );
int audio_dse_dec(void *pv_data, int n_size );

const static int KK_OK = 0;
const static int KK_ERR_H264_SEI_DEC_PARA = 20001;
const static int KK_ERR_H264_PAYLOAD_DEC_PARA = 20010;
const static int KK_ERR_LAYOUT_DATA_DEC_PARA = 20020;
// const static int KK_ERR_H264_SEI_DEC_PARA = 20001;
// const static int KK_ERR_H264_SEI_DEC_PARA = 20001;
const static int KK_ERR_AUD_DSE_DEC_PARA = 20101;
const static int KK_ERR_AUD_DSE_PAYLOAD_DEC_PARA = 20111;
// const static int KK_ERR_H264_SEI_DEC_PARA = 20001;

const static int KK_ERR_AUD_DSE_VOL_DEC_PARA = 20121;
const static int KK_ERR_AUD_DSE_VOL_DEC_VOL_LEN = 20122;
const static int KK_ERR_AUD_DSE_VOL_DEC_VOLUME = 20123;



static t_kk_sei_dse g_sei_dse;
static BOOL is_h264_nal_start_code(u8* pdat, int len)
{
    if ( (NULL == pdat) || (len < 4) )
    {
        printf("[%s:%d]-parameter error, data:%p, size:%d\n", FL, pdat, len);
        return FALSE;
    }

    if (0 != *pdat)
    {
        return FALSE;
    }
    if (0 != *(pdat+1))
    {
        return FALSE;
    }
    if (0 != *(pdat+2))
    {
        return FALSE;
    }
    if (1 != *(pdat+3))
    {
        return FALSE;
    }

    return TRUE;
}


#define  KK_MOVE_BYTE(ret_dat, psrc, src_size)\
    {\
        ret_dat = *psrc;\
        psrc++;\
        src_size--;\
    }


#define  KK_MOVE_WORD(ret_dat, tmp_dat, psrc, src_size)\
    tmp_dat = *(u16 *)psrc;\
    ret_dat = BSWAP_16(tmp_dat);\
    psrc+=2;\
    src_size-=2;


#define  KK_MOVE_BUF(dst_ptr, data_len, psrc, src_size)\
    dst_ptr = malloc(data_len);\
    memcpy(dst_ptr, psrc, data_len);\
    psrc+=data_len;\
    n_size-=data_len;


static int layout_data_dec(const void *pv_data, int n_size, int* out_size)
{
    u8* psrc = pv_data;
    if ( (NULL == psrc) || (n_size < 1) )
    {
        printf("[%s:%d]-parameter error, data:%p, size:%d\n", FL, psrc, n_size);
        return KK_ERR_LAYOUT_DATA_DEC_PARA;
    }

    u16 tmp_len = 0;
    u16 data_len = 0;
    t_kk_layout_data* layout_dat = malloc(sizeof(t_kk_layout_data));
    memset(&layout_dat, 0, sizeof(layout_dat));

    KK_MOVE_WORD(layout_dat->length, tmp_len, psrc, n_size);
    KK_MOVE_BYTE(layout_dat->uid_len, psrc, n_size);
    KK_MOVE_WORD(layout_dat->data_data_len, tmp_len, psrc, n_size);

    KK_MOVE_BUF(layout_dat->pdata, layout_dat->data_data_len, psrc, n_size);

    KK_MOVE_BYTE(layout_dat->flag, psrc, n_size);
    KK_MOVE_BYTE(layout_dat->alpha, psrc, n_size);
    KK_MOVE_BYTE(layout_dat->zorder, psrc, n_size);
    KK_MOVE_WORD(layout_dat->src_width, tmp_len, psrc, n_size);
    KK_MOVE_WORD(layout_dat->src_height, tmp_len, psrc, n_size);

    KK_MOVE_WORD(layout_dat->tcrop.x, tmp_len, psrc, n_size);
    KK_MOVE_WORD(layout_dat->tcrop.y, tmp_len, psrc, n_size);
    KK_MOVE_WORD(layout_dat->tcrop.width, tmp_len, psrc, n_size);
    KK_MOVE_WORD(layout_dat->tcrop.height, tmp_len, psrc, n_size);

    KK_MOVE_WORD(layout_dat->tdest.x, tmp_len, psrc, n_size);
    KK_MOVE_WORD(layout_dat->tdest.y, tmp_len, psrc, n_size);
    KK_MOVE_WORD(layout_dat->tdest.width, tmp_len, psrc, n_size);
    KK_MOVE_WORD(layout_dat->tdest.height, tmp_len, psrc, n_size);
}

static int app_data_payload_dec(const void *pv_data, int n_size)
{
    u8* psrc = pv_data;
    if ( (NULL == psrc) || (n_size < 1) )
    {
        printf("[%s:%d]-parameter error, data:%p, size:%d\n", FL, psrc, n_size);
        return KK_ERR_H264_PAYLOAD_DEC_PARA;
    }
    int data_cnt = g_sei_dse.t_vid_sei.t_sei_app_data_payload.data_count = *psrc;
    psrc++;
    n_size--;
    int id;
    for (id = 0; id < data_cnt; id++)
    {
        s32 new_size = 0;
        psrc = layout_data_dec(psrc, n_size, &new_size);
        n_size = new_size;
    }
}


static int h264_payload_dec(const void *pv_data, int n_size)
{
    u8* psrc = pv_data;
    if ( (NULL == psrc) || (n_size < 1) )
    {
        printf("[%s:%d]-parameter error, data:%p, size:%d\n", FL, psrc, n_size);
        return KK_ERR_H264_PAYLOAD_DEC_PARA;
    }
    u16 len = 0;
    u8 ptype = 0; 
    KK_MOVE_WORD(g_sei_dse.t_vid_sei.t_sei_payload.length, len, psrc, n_size);
    KK_MOVE_BYTE(ptype, psrc, n_size);
    g_sei_dse.t_vid_sei.t_sei_payload.payload_type = ptype;
    printf("sei payload type:%d\n", ptype);

    switch (ptype)
    {
        case PTYPE_KK_APP:
        break;
        case PTYPE_APP_DATA:
            app_data_payload_dec(psrc, n_size);
        break;
        case PTYPE_CFG_DATA:
            KK_MOVE_BYTE(g_sei_dse.t_vid_sei.tcfg_data.flag, psrc, n_size);
        break;
        case PTYPE_LAYOUT_DATA:
        break;
        default:
        printf("unnkow type:%d\n", ptype);
        break;
    }

// 1:kk app data
// 2:app data
// 3:config data
// 4:layout data
}

typedef enum emKK_SEI_PAYLOAD_TYPE
{
    PTYPE_KK_APP = 1,
    PTYPE_APP_DATA,
    PTYPE_CFG_DATA,
    PTYPE_LAYOUT_DATA,
}eKK_SEI_PAYLOAD_TYPE;

const static int KK_SEI_PAYLOAD_TYPE_YY = 1;

int h264_sei_dec(const void *pv_data, int n_size)
{
    u8* psrc = pv_data;
    int ret = KK_OK;
    FILE *fp = NULL;
    if ( (NULL == psrc) || (n_size < 1) )
    {
        printf("[%s:%d]-parameter error, data:%p, size:%d\n", FL, psrc, n_size);
        return KK_ERR_H264_SEI_DEC_PARA;
    }

    if (is_h264_nal_start_code(psrc, n_size))
    {
        psrc += 4;
        u8 nal_type = *psrc;
        u8 payload_type = *(psrc+1);
        int step = 4+1;
 
        switch (nal_type)
        {
            case 6:
                printf("nal-type:is h264\n");
                if (201 == payload_type)
                {
                    printf("nal-type:is user-define-payload\n");
                    psrc++;
                    step++;
                    int payload_size = 0;
                    while (!feof(fp) && payload_size % 0xFF == 0) {
                        payload_size += fgetc(fp);
                        printf("payload_size 111= %i\n", payload_size);
                    }
                    printf("payload_size = %i\n", payload_size);
                    ret = h264_payload_dec(psrc, payload_size);
                }
            break;
            case 78:
                printf("nal-type:is h265\n");
                if (201 == payload_type)
                {
                    printf("nal-type:is user-define-payload\n");
                }
            break;
            default:
                printf("unknow-nal-type:%d\n", nal_type);
            break;
        }
    }
    else
    {
        psrc++;
    }
    return ret;
}


static int aud_dse_vol_para_dec(const void *pv_data, int n_size, int* out_size)
{
    u8* psrc = pv_data;
    s32 ret = KK_OK;
    if ( (NULL == psrc) || (n_size < 1) )
    {
        printf("[%s:%d]-parameter error, data:%p, size:%d\n", FL, psrc, n_size);
        return KK_ERR_AUD_DSE_VOL_DEC_PARA;
    }

    u16 tmp = 0;
    u16 vol_len = 0;
    KK_MOVE_WORD(vol_len, tmp, psrc, n_size);
    if ( (vol_len < 2) || (vol_len > 500) )
    {
        printf("[%s:%d][E]volume length:%d, must in[2-500]\n", FL, vol_len);
        return KK_ERR_AUD_DSE_VOL_DEC_VOL_LEN;
    }
    g_sei_dse.t_aud_dse.t_aud_vol_para.length = vol_len;
    KK_MOVE_BYTE(tmp, psrc, n_size);
    g_sei_dse.t_aud_dse.t_aud_vol_para.uid_len = tmp;
    KK_MOVE_BUF(g_sei_dse.t_aud_dse.t_aud_vol_para.puid_str, tmp, psrc, n_size);

    KK_MOVE_BYTE(tmp, psrc, n_size);
    if (tmp > 100)
    {
        printf("[%s:%d][E]volume:%d, must in[0-100]\n", FL, tmp);
        return KK_ERR_AUD_DSE_VOL_DEC_VOLUME;
    }
    g_sei_dse.t_aud_dse.t_aud_vol_para.volume = tmp;
    return ret;
}



static int aud_dse_vol_data_dec(const void *pv_data, int n_size)
{
    u8* psrc = pv_data;
    int ret = KK_OK;
    if ( (NULL == psrc) || (n_size < 1) )
    {
        printf("[%s:%d]-parameter error, data:%p, size:%d\n", FL, psrc, n_size);
        return KK_ERR_AUD_DSE_PAYLOAD_DEC_PARA;
    }
    u16 tmp_len = 0;
    int data_cnt = 0;
    KK_MOVE_BYTE(data_cnt, psrc, n_size);
    g_sei_dse.t_aud_dse.t_aud_vol_data.volume_cnt = data_cnt;
    int id;
    for (id = 0; id < data_cnt; id++)
    {
        s32 new_size = 0;
        psrc = aud_dse_vol_para_dec(psrc, n_size, &new_size);
        n_size = new_size;
    }
    
    return ret;
}

static int aud_dse_payload_dec(const void *pv_data, int n_size)
{
    u8* psrc = pv_data;
    int ret = KK_OK;
    if ( (NULL == psrc) || (n_size < 1) )
    {
        printf("[%s:%d]-parameter error, data:%p, size:%d\n", FL, psrc, n_size);
        return KK_ERR_AUD_DSE_PAYLOAD_DEC_PARA;
    }
    u16 tmp_len = 0;
    KK_MOVE_WORD(g_sei_dse.t_aud_dse.t_aud_dse_payload.length, tmp_len, psrc, n_size);
    KK_MOVE_BYTE(g_sei_dse.t_aud_dse.t_aud_dse_payload.type, psrc, n_size);
    aud_dse_vol_data_dec(psrc, n_size);
    return ret;
}


int aud_dse_dec(const void *pv_data, int n_size)
{
    u8* psrc = pv_data;
    int ret = KK_OK;
    if ( (NULL == psrc) || (n_size < 1) )
    {
        printf("[%s:%d]-parameter error, data:%p, size:%d\n", FL, psrc, n_size);
        return KK_ERR_AUD_DSE_DEC_PARA;
    }

    u8 aud_dse_id = 0;
    KK_MOVE_BYTE(aud_dse_id, psrc, n_size);
    g_sei_dse.t_aud_dse.t_aud_dse_info.id = aud_dse_id;
    if (KK_AUDIO_DSE_TAG == aud_dse_id)
    {
        KK_MOVE_BYTE(g_sei_dse.t_aud_dse.t_aud_dse_info.count, psrc, n_size);
        KK_MOVE_BYTE(g_sei_dse.t_aud_dse.t_aud_dse_info.esc_count, psrc, n_size);
        int pay_load_size = g_sei_dse.t_aud_dse.t_aud_dse_info.count
            +g_sei_dse.t_aud_dse.t_aud_dse_info.esc_count;
        aud_dse_payload_dec(psrc, pay_load_size);
    }
    
    return ret;
}



int main(void)
{
    const char *pch_src = "We#Love#YY";
    memset(&g_sei_dse, 0, sizeof(g_sei_dse));
    h264_sei_dec(pch_src, 122);
    
    system("pause");
    return 0;
}


// ./sei_parser.exe  C:\\Users\\isuke\\wk\\media\\video\\tst\\trailer.h264
