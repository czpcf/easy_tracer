# easy_tracer

在建中。。。。。。。

structure:

    integrator（没有抽象，直接写代码）
    camera
    object（注：用一个标识符来区分光源/非光源）
        geometry（求交）
        texture
        bsdf
    accelerator（init、查询）
        object
        box（bounding box）

其它一些东西:

    io（输入输出，parser）
    tenstor（vec3, mat3）

TODO：

    1. OpenMP
    2. PT，PM
    3. 各种照相机
    4. 一些奇怪的采样方法（比如说ReSTIR）
    5. 后期去噪
    6. cuda
    7. （浸泡在液体中的）简单的体积光（用光追做体积光不是一个好选择）