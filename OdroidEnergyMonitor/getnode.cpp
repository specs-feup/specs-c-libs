#include "getnode.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>

GetNode::GetNode()
{
    for (int i = 0; i < 8; i++) {
		usage[i] = 0;
	}

    for (int i = 0; i < 8; i++) {
		std::stringstream builder;
		builder << "/sys/devices/system/cpu/cpu" << i << "/cpufreq/cpuinfo_cur_freq";
        cpu_node_list[i] = builder.str();
    }
}

int GetNode::GetGPUCurFreq()
{
	int freqi = -1;
	std::ifstream fp(GPUFREQ_NODE, std::ios_base::binary);
	if (!fp.good()) {
		return -1;
	}
	fp >> freqi;
	
    return freqi;
}

int GetNode::GetCPUCurFreq(int cpuNum)
{
	int freqi = -1;
	std::ifstream fp(cpu_node_list[cpuNum].c_str(), std::ios_base::binary);
	if (!fp.good()) {
		return -1;
	}
	fp >> freqi;
	
    return freqi;
}

int GetNode::GetCPUTemp(int cpuNum)
{
	std::ifstream fp(TEMP_NODE, std::ios_base::binary);
	if (!fp.good()) {
		return -1;
	}
	
	int temp = -1;
	
    for (int i = 0; i <= cpuNum; i++) {
		temp = -1;
		fp >> temp;
	}

    return temp;
}

int GetNode::open_sensor(const char *node, sensor_t *sensor)
{
    if ((sensor->fd = open(node, O_RDWR)) < 0)
        std::cerr << node << "Open Fail" << std::endl;

    return sensor->fd;
}

int GetNode::OpenINA231()
{
    if (open_sensor(DEV_SENSOR_ARM, &sensor[SENSOR_ARM]) < 0)
        return -1;
    if (open_sensor(DEV_SENSOR_MEM, &sensor[SENSOR_MEM]) < 0)
        return -1;
    if (open_sensor(DEV_SENSOR_KFC, &sensor[SENSOR_KFC]) < 0)
        return -1;
    if (open_sensor(DEV_SENSOR_G3D, &sensor[SENSOR_G3D]) < 0)
        return -1;

    if (read_sensor_status(&sensor[SENSOR_ARM]))
        return -1;
    if (read_sensor_status(&sensor[SENSOR_MEM]))
        return -1;
    if (read_sensor_status(&sensor[SENSOR_KFC]))
        return -1;
    if (read_sensor_status(&sensor[SENSOR_G3D]))
        return -1;

    if (!sensor[SENSOR_ARM].data.enable)
        enable_sensor(&sensor[SENSOR_ARM], 1);
    if (!sensor[SENSOR_MEM].data.enable)
        enable_sensor(&sensor[SENSOR_MEM], 1);
    if (!sensor[SENSOR_KFC].data.enable)
        enable_sensor(&sensor[SENSOR_KFC], 1);
    if (!sensor[SENSOR_G3D].data.enable)
        enable_sensor(&sensor[SENSOR_G3D], 1);

    return 0;
}

void GetNode::CloseINA231()
{
    if (sensor[SENSOR_ARM].data.enable)
        enable_sensor(&sensor[SENSOR_ARM], 0);
    if (sensor[SENSOR_MEM].data.enable)
        enable_sensor(&sensor[SENSOR_MEM], 0);
    if (sensor[SENSOR_KFC].data.enable)
        enable_sensor(&sensor[SENSOR_KFC], 0);
    if (sensor[SENSOR_G3D].data.enable)
        enable_sensor(&sensor[SENSOR_G3D], 0);

    close_sensor(&sensor[SENSOR_ARM]);
    close_sensor(&sensor[SENSOR_MEM]);
    close_sensor(&sensor[SENSOR_KFC]);
    close_sensor(&sensor[SENSOR_G3D]);
}

void GetNode::GetINA231()
{
    read_sensor(&sensor[SENSOR_ARM]);
    read_sensor(&sensor[SENSOR_MEM]);
    read_sensor(&sensor[SENSOR_KFC]);
    read_sensor(&sensor[SENSOR_G3D]);

    armuV = (float)(sensor[SENSOR_ARM].data.cur_uV / 100000) / 10;
    armuA = (float)(sensor[SENSOR_ARM].data.cur_uA / 1000) / 1000;
    armuW = sensor[SENSOR_ARM].data.cur_uW / 1000000.0f;

    memuV = (float)(sensor[SENSOR_MEM].data.cur_uV / 100000) / 10;
    memuA = (float)(sensor[SENSOR_MEM].data.cur_uA / 1000) / 1000;
    memuW = sensor[SENSOR_MEM].data.cur_uW / 1000000.0f;

    kfcuV = (float)(sensor[SENSOR_KFC].data.cur_uV / 100000) / 10;
    kfcuA = (float)(sensor[SENSOR_KFC].data.cur_uA / 1000) / 1000;
    kfcuW = sensor[SENSOR_KFC].data.cur_uW / 1000000.0f;

    g3duV = (float)(sensor[SENSOR_G3D].data.cur_uV / 100000) / 10;
    g3duA = (float)(sensor[SENSOR_G3D].data.cur_uA / 1000) / 1000;
    g3duW = sensor[SENSOR_G3D].data.cur_uW / 1000000.0f;

}

void GetNode::enable_sensor(sensor_t *sensor, unsigned char enable)
{
    if (sensor->fd > 0) {
        sensor->data.enable = enable ? 1 : 0;
        if (ioctl(sensor->fd, INA231_IOCSSTATUS, &sensor->data) < 0)
            std::cerr << "IOCTL Error" << std::endl;
    }
}

int GetNode::read_sensor_status(sensor_t *sensor)
{
    if (sensor->fd > 0) {
        if (ioctl(sensor->fd, INA231_IOCGSTATUS, &sensor->data) < 0)
            std::cerr << sensor->data.name << "IOCTL Error" << std::endl;
    }
    return 0;
}

void GetNode::read_sensor(sensor_t *sensor)
{
    if (sensor->fd > 0) {
        if (ioctl(sensor->fd, INA231_IOCGREG, &sensor->data) < 0)
            std::cerr << sensor->data.name << "IOCTL Error!" << std::endl;
    }
}

void GetNode::close_sensor(sensor_t *sensor)
{
    if (sensor->fd > 0)
        close(sensor->fd);
}

int GetNode::calUsage(int cpu_idx, int user, int nice, int system, int idle)
{
    long total = 0;
    long usage = 0;
    int diff_user, diff_system, diff_idle;

    diff_user = mOldUserCPU[cpu_idx] - user;
    diff_system = mOldSystemCPU[cpu_idx] - system;
    diff_idle = mOldIdleCPU[cpu_idx] - idle;

    total = diff_user + diff_system + diff_idle;
    if (total != 0)
        usage = diff_user * 100 / total;

    mOldUserCPU[cpu_idx] =user;
    mOldSystemCPU[cpu_idx] = system;
    mOldIdleCPU[cpu_idx] = idle;

    return usage;
}

int GetNode::GetCPUUsage(void)
{
    char buf[80] = {0,};
    char cpuid[8] = "cpu";
    int findCPU = 0;
    int user, system, nice, idle;
    FILE *fp;
    int cpu_index = 0;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
        return 0;

    while (fgets(buf, 80, fp)) {
        char temp[4] = "cpu";
        temp[3] = '0' + cpu_index;
        if (!strncmp(buf, temp, 4)) {
            findCPU = 1;
            sscanf(buf, "%s %d %d %d %d",
                   cpuid, &user, &nice, &system, &idle);
            usage[cpu_index] = calUsage(cpu_index, user, nice, system, idle);
            cpu_index++;
        }
        if (!strncmp(buf, "intr", 4))
            break;
        if (findCPU == 0)
            mOldUserCPU[cpu_index] = mOldSystemCPU[cpu_index] = mOldIdleCPU[cpu_index] = 0;
        else
            findCPU = 0;
    }

    fclose(fp);

    return 0;
}
