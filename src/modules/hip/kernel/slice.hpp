#include <hip/hip_runtime.h>
#include "rpp_hip_common.hpp"

__global__ void slice_tensor(float *srcPtr,
                                           uint2 srcStridesNH,
                                           float *dstPtr,
                                           uint2 dstStridesNH,
                                           int *srcLengthTensor,
                                           float *anchorTensor,
                                           float *shapeTensor,
                                           float fillValue,
                                           RpptOutOfBoundsPolicy policyType)
{
    int id_x = (hipBlockIdx_x * hipBlockDim_x + hipThreadIdx_x);
    int id_y = hipBlockIdx_y * hipBlockDim_y + hipThreadIdx_y;
    int id_z = hipBlockIdx_z * hipBlockDim_z + hipThreadIdx_z;
    int stride = id_z * 2;

    if (id_y >= shapeTensor[stride] || id_x >= shapeTensor[stride + 1])
    {
        return;
    }

    uint srcIdx = (id_z * srcStridesNH.x) + ((id_y + anchorTensor[stride]) * srcStridesNH.y) + (id_x + anchorTensor[stride + 1]);
    uint dstIdx = (id_z * dstStridesNH.x) + (id_y * dstStridesNH.y) + id_x;
    
    dstPtr[dstIdx] = srcPtr[srcIdx];
}

RppStatus hip_exec_slice_tensor(Rpp32f *srcPtr,
                                RpptDescPtr srcDescPtr,
                                Rpp32f *dstPtr,
                                RpptDescPtr dstDescPtr,
                                Rpp32s axisMask,
                                Rpp32f *fillValues,
                                bool normalizedAnchor,
                                bool normalizedShape,
                                RpptOutOfBoundsPolicy policyType,
                                rpp::Handle& handle)
{
    int localThreads_x = LOCAL_THREADS_X;
    int localThreads_y = LOCAL_THREADS_Y;
    int localThreads_z = LOCAL_THREADS_Z;
    int globalThreads_x = dstDescPtr->strides.hStride;
    int globalThreads_y = dstDescPtr->h;
    int globalThreads_z = handle.GetBatchSize();

    hipLaunchKernelGGL(slice_tensor,
                       dim3(ceil((float)globalThreads_x/localThreads_x), ceil((float)globalThreads_y/localThreads_y), ceil((float)globalThreads_z/localThreads_z)),
                       dim3(localThreads_x, localThreads_y, localThreads_z),
                       0,
                       handle.GetStream(),
                       srcPtr,
                       make_uint2(srcDescPtr->strides.nStride, srcDescPtr->strides.hStride),
                       dstPtr,
                       make_uint2(dstDescPtr->strides.nStride, dstDescPtr->strides.hStride),
                       handle.GetInitHandle()->mem.mgpu.int2Arr[0].intmem,
                       handle.GetInitHandle()->mem.mgpu.float2Arr[1].floatmem,
                       handle.GetInitHandle()->mem.mgpu.float2Arr[2].floatmem,
                       *fillValues,
                       policyType);

    return RPP_SUCCESS;
}