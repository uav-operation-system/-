# GUIDANCE SDK-API 接口名称和参数

image\_data

描述: 定义图像的数据结构。每个方向的深度图与双目灰度图中的左图对齐。

```java
typedef struct _image_data
{
    unsigned int     frame_index;                   // frame index
    unsigned int     time_stamp;                    // time stamp of image captured in ms
    char *m_greyscale_image_left[CAMERA_PAIR_NUM];// greyscale image of left       camera
    char *m_greyscale_image_right[CAMERA_PAIR_NUM];   // greyscale image of right camera
    char   *m_depth_image[CAMERA_PAIR_NUM];          // depth image in *128 meters
    char  *m_disparity_image[CAMERA_PAIR_NUM];      // disparity image in *16 pixels
}image_data;
```

解释:

1、m\_greyscale\_image\_left和m\_greyscale\_image\_right都是宽320，高240的8比特灰度图。

2、m\_depth\_image是宽320，高240的16比特深度图，每两个字节描述一个点的深度，低7位为小数位，高9位为整数位。

3、m\_disparity\_image是宽320，高240的16比特视差图，每两个字节描述一个点的深度，低4位为小数位，高12位为整数位。

VI、ultrasonic\_data

描述:
定义超声波的数据结构。ultrasonic是超声波传感器检测到的最近物体的距离，单位是mm。reliability是该距离测量的可信度，1为可信，0为不可信。由于观测数据存在噪声，建议对数据进行滤波后再使用。

```java
typedef struct _ultrasonic_data
{
    unsigned int     frame_index;    // corresponse frame index
    unsigned int     time_stamp;     // time stamp of corresponse image captured in ms
    short        ultrasonic[CAMERA_PAIR_NUM];            // distance in mm. -1 means invalid measurement.
    unsigned short   reliability[CAMERA_PAIR_NUM];   // reliability of the distance data
}ultrasonic_data;
```

VII、velocity

描述: 定义体坐标系下的速度。单位是mm/s。

```java
typedef struct _velocity
{
    unsigned int     frame_index;        // corresponse frame index
    unsigned int     time_stamp;         // time stamp of corresponse image captured in ms
    short            vx;                 // velocity of x in mm/s
    short            vy;                 // velocity of y in mm/s
    short            vz;                 // velocity of z in mm/s
}velocity;
```

VIII、obstacle\_distance

描述: 定义由视觉和超声波融合得到的障碍物距离。单位是cm。

```java
typedef struct _obstacle_distance
{
    unsigned int     frame_index;    // corresponse frame index
    unsigned int     time_stamp;     // time stamp of corresponse image captured in ms
    unsigned short   distance[CAMERA_PAIR_NUM];  // distance of obstacle in cm
}obstacle_distance;
```

IX、imu

描述: 定义IMU数据结构。加速度单位为m/s\^2

```java
typedef struct _imu
{
    unsigned int     frame_index;             //相应的帧索引
    unsigned int     time_stamp;           //以ms为单位捕获的对应图像的时间戳
    float            acc_x;                   // acceleration of x in unit of m/s^2
    float            acc_y;                   // acceleration of y in unit of m/s^2
    float            acc_z;                   // acceleration of z in unit of m/s^2
    float            q[4];                    // quaternion: [w,x,y,z]
}imu;
```

X、stereo\_cali

描述: 摄像头的标定参数。如果某个方向的传感器不在线，则所有值为0.

```java
typedef struct _stereo_cali
{
    float cu;                // 焦点中心的x位置，以像素为单位
    float cv;                // 焦点中心的y位置，以像素为单位
    float focal;             // 焦距以像素为单位
    float baseline;        // 以米为单位的立体相机基线
    _stereo_cali() { }
    _stereo_cali(float _cu, float _cv, float _focal, float _baseline)
    {
        cu = _cu, cv = _cv;
        focal = _focal, baseline = _baseline;
    }
}stereo_cali;
```

XI、exposure\_param

描述: 摄像头的曝光参数。当m\_expo\_time =
m\_expected\_brightness=0时，变成默认的自动曝光控制

```java
typedef struct _exposure_param
{
float      m_step;        // 自动曝光控制（AEC）的调整步骤。 默认值为10。
    float   m_exposure_time;// 持续曝光时间以微秒为单位。 范围是0.1~20。 默认值为7.25。
unsigned int  m_expected_brightness;   // AEC的恒定预期亮度。 范围是50~200。 默认值为85。
unsigned int  m_is_auto_exposure;     // 1：自动曝光; 0：持续曝光
    int           m_camera_pair_index; // index of Guidance Sensor
    _exposure_param(){
        m_step = 10;
        m_exposure_time = 7.68;
        m_expected_brightness = 85;
        m_is_auto_exposure = 1;
        m_camera_pair_index = 1;
    }
}exposure_param;
```

XII、motion

描述: 定义全局位置数据结构。位置单位为m，速度单位为m/s.

```java
typedef struct _motion
{
    unsigned int     frame_index;
    unsigned int     time_stamp;
    int                corresponding_imu_index;
    float            q0;
    float            q1;
    float            q2;
    float            q3;
    int              attitude_status;  // 0:invalid; 1:valid
    float            position_in_global_x;  // position in global frame: x
    float            position_in_global_y;  // position in global frame: y
    float            position_in_global_z;  // position in global frame: z
    int              position_status; // lower 3 bits are confidence. 0:invalid; 1:valid
    float            velocity_in_global_x;  // velocity in global frame: x
    float            velocity_in_global_y;  // velocity in global frame: y
    float            velocity_in_global_z;  // velocity in global frame: z
    int              velocity_status; // lower 3 bits are confidence. 0:invalid; 1:valid
    float            reserve_float[8];
    int              reserve_int[4];
    float            uncertainty_location[3];// uncertainty of position
    float            uncertainty_velocity[3];// uncertainty of velocity
} motion;
```
