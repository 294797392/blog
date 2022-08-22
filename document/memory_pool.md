# 内存池

有时候我们会需要频繁的开辟/释放内存，这样会给程序带来一些性能上的影响，特别是对于带有自动垃圾回收功能（GC）的编程语言（比如C#，Java）来说，每次执行垃圾内存的回收对性能还是很有影响的，此时可以使用内存池来应对这种需要频繁开辟/释放内存的场景。

内存池的主要实现思路是把开辟后的内存块用完缓存起来，下次如果要开辟新的内存块，那么复用缓存起来的内存块。

但是要实现一个内存池首先有几个问题需要解决：
1. 内存池需要支持可以开辟任意长度的内存块
2. 内存池需要在多线程的环境下工作

下面针对这几个问题展开讨论。

*** 开辟任意长度的内存空间

这里介绍一种做法是调用者输入一个内存池里要开辟的最长的内存块的长度，根据这个长度去创建一个内存桶列表，每个内存桶里的内存块大小是固定的，然后为每个桶指定内存块的数量，如图所示：

|------------||----------||----------||----------||----------||----------||----------||----------|
|	bucket1  ||	bucket2  ||	bucket3	 ||	bucket4  ||	bucket5  ||	bucket6  ||	bucket7	 ||	bucket8  |
|	16bytes	 ||	32bytes	 ||	64bytes	 ||	128bytes ||	256bytes ||	512bytes ||	1MB  	 ||	  2MB 	 |
|------------||----------||----------||----------||----------||----------||----------||----------|

其中的一个问题是内存的分配粒度要设置多少（每个内存块的大小应该是分配粒度的整数倍），这里我们使用的分配粒度是16字节。
可以看出来要开辟的内存块越大，那么需要的桶的数量就越多。

下面提供两个方法用来计算内存块大小与桶的数量之间的关系（摘抄自.net core的ArrayPool源码）：

        /// <summary>
        /// 根据bufferSize计算bucket的索引
        /// </summary>
        /// <param name="bufferSize"></param>
        /// <returns></returns>
        internal static int SelectBucketIndex(int bufferSize)
        {
            uint bitsRemaining = ((uint)bufferSize - 1) >> 4;

            int poolIndex = 0;
            if (bitsRemaining > 0xFFFF) { bitsRemaining >>= 16; poolIndex = 16; }
            if (bitsRemaining > 0xFF) { bitsRemaining >>= 8; poolIndex += 8; }
            if (bitsRemaining > 0xF) { bitsRemaining >>= 4; poolIndex += 4; }
            if (bitsRemaining > 0x3) { bitsRemaining >>= 2; poolIndex += 2; }
            if (bitsRemaining > 0x1) { bitsRemaining >>= 1; poolIndex += 1; }

            return poolIndex + (int)bitsRemaining;
        }

        /// <summary>
        /// 根据bucket索引计算bucket里的每个block的大小
        /// </summary>
        /// <param name="binIndex"></param>
        /// <returns></returns>
        internal static int GetMaxSizeForBucket(int binIndex)
        {
            int maxSize = 16 << binIndex;
            return maxSize;
        }


*** 多线程环境下的线程同步






