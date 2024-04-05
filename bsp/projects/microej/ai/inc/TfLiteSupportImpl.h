/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TFLITE_SUPPORT_IMPL
#define TFLITE_SUPPORT_IMPL

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "fsl_debug_console.h"
class TfLiteSupportImpl
{
    public:
        TfLiteSupportImpl(const void *modelData, size_t arena_size);

        ~TfLiteSupportImpl();

        template<typename T>
        T* GetInputData(int idx) { return interpreter->typed_input_tensor<T>(idx); }
        template<typename T>
        T* GetOutputData(int idx) { return interpreter->typed_output_tensor<T>(idx); }
        TfLiteTensor* GetInputTensor(int idx) { return interpreter->input_tensor(idx); }
        TfLiteTensor* GetOutputTensor(int idx) { return interpreter->output_tensor(idx); }

        int RunInference();

    private:
        const uint8_t *model_data;
        tflite::MicroOpResolver* micro_op_resolver;
        const tflite::Model *model;
        tflite::MicroInterpreter *interpreter;
        uint8_t *tensor_arena;
};

#endif
