## getopt
头文件：getopt.h

## getopt_long
头文件：getopt.h

## gettimeofday
原型：
参数：
返回值：

1. 获取时间差
double GetElapsed(struct timeval start, struct timeval end)
{
	uint64_t start_usec = start.tv_sec * 1000000 + start.tv_usec;
	uint64_t end_usec = end.tv_sec * 1000000 + end.tv_usec;
	return (end_usec - start_usec) / 1000;
}

2. 获取系统启动以来所经过的毫秒数
double GetTime()
{
	struct timeval time;
    gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000 + time.tv_usec) / 1000;
}
