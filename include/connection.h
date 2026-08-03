#pragma once
#include <linux/input.h>
#include <stddef.h>

static constexpr char SOCKET_NAME[] = "/tmp/loggertest.socket";
static constexpr size_t MAX_KEY_SIZE = 32;
static constexpr size_t BUFFER_SIZE = sizeof(input_event) + MAX_KEY_SIZE + 1;
