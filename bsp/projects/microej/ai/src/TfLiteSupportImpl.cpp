/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "TfLiteSupportImpl.h"
#include "fsl_debug_console.h"
#include "tensorflow/lite/micro/micro_op_resolver.h"

TfLiteSupportImpl::TfLiteSupportImpl(const void *modelData, size_t arena_size)
{
    model = tflite::GetModel(modelData);
    if(model == nullptr)
    {
        PRINTF("ERROR: failed to get model\r\n");
    }
    assert (model != nullptr);

    // Allocate arena tensor memory
    tensor_arena = new uint8_t[arena_size];
    if(tensor_arena == nullptr)
    {
        PRINTF("ERROR: failed to allocate tensor arena\r\n");
    }
    assert (tensor_arena != nullptr);

    extern tflite::MicroOpResolver* AI_GetOpsResolver();
    micro_op_resolver = AI_GetOpsResolver();
    if(micro_op_resolver == nullptr)
    {
        PRINTF("ERROR: failed to get micro operations resolver\r\n");
    }
    assert (micro_op_resolver != nullptr);

    // Build an interpreter to run the model with
    interpreter = new tflite::MicroInterpreter(model, *micro_op_resolver,
            tensor_arena, arena_size);
    if(interpreter == nullptr)
    {
        PRINTF("ERROR: failed to initialize interpreter\r\n");
    }
    assert (interpreter != nullptr);

    // Allocate memory from the tensor_arena for the model's tensor
    if(interpreter->AllocateTensors() != kTfLiteOk)
    {
        PRINTF("Allocate tensor failed\r\n");
        assert (1 == 0);
    }
}

TfLiteSupportImpl::~TfLiteSupportImpl()
{
    delete [] tensor_arena;
    delete(interpreter);
}

int TfLiteSupportImpl::RunInference()
{
    int ret = 0;

    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk)
    {
        PRINTF("Interpreter invocation failed\r\n");
        ret = kTfLiteError;
    }
    return ret;
}
