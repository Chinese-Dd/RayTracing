C++实现的简单光线追踪器.
相关配置文件及数据来自于:[南加利福尼亚大学CSCI480的课程试验](http://run.usc.edu/cs480-s13/assignments/assign3/assign3.html).
参考网址:[光线追踪技术的理论和实践(面向对象)]http://blog.csdn.net/zhangci226/article/details/5664313,
[光线追踪(RayTracing)算法理论与实践(一)入门]http://blog.csdn.net/silangquan/article/details/8176855.


2017/12/23未解决的问题:
采用OpenMP #pragma omp parallel for后输出 进度百分比有问题.不采用OpenMP #pragma omp parallel for,执行最后一个任务太耗时.
