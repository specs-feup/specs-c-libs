#ifndef GETNODE_H
#define GETNODE_H

#include <sys/ioctl.h>
#include <string>

#define GPUFREQ_NODE    "/sys/devices/11800000.mali/clock"
#define TEMP_NODE "/sys/devices/platform/exynos5-tmu/temp"

typedef struct ina231_iocreg__t {
    char name[20];
    unsigned int enable;
    unsigned int cur_uV;
    unsigned int cur_uA;
    unsigned int cur_uW;
} ina231_iocreg_t;

typedef struct sensor__t {
    int  fd;
    ina231_iocreg_t data;
} sensor_t;

#define INA231_IOCGREG      _IOR('i', 1, ina231_iocreg_t *)
#define INA231_IOCSSTATUS   _IOW('i', 2, ina231_iocreg_t *)
#define INA231_IOCGSTATUS   _IOR('i', 3, ina231_iocreg_t *)

#define DEV_SENSOR_ARM  "/dev/sensor_arm"
#define DEV_SENSOR_MEM  "/dev/sensor_mem"
#define DEV_SENSOR_KFC  "/dev/sensor_kfc"
#define DEV_SENSOR_G3D  "/dev/sensor_g3d"

enum    {
    SENSOR_ARM = 0,
    SENSOR_MEM,
    SENSOR_KFC,
    SENSOR_G3D,
    SENSOR_MAX
};


class GetNode
{
public:
    GetNode();
    std::string cpu_node_list[8];
    float armuV,armuA, armuW;
    float g3duV, g3duA, g3duW;
    float kfcuV, kfcuA, kfcuW;
    float memuV, memuA, memuW;
    int usage[8];

    int GetGPUCurFreq(void);
    int GetCPUCurFreq(int cpuNum);
    int GetCPUTemp(int cpuNum);
    int GetCPUUsage(void);
    int calUsage(int cpu_idx, int user, int nice, int system, int idle);

    int open_sensor(const char *node, sensor_t *sensor);
    void close_sensor(sensor_t *sensor);
    void enable_sensor(sensor_t *sensor, unsigned char enable);
    int read_sensor_status(sensor_t *sensor);
    void read_sensor(sensor_t *sensor);

    int OpenINA231(void);
    void CloseINA231(void);
    void GetINA231(void);

private:
    sensor_t sensor[SENSOR_MAX];
    int mOldUserCPU[8];
    int mOldSystemCPU[8];
    int mOldIdleCPU[8];

};

#endif // GETNODE_H
