//
// Created by Angus Goucher on 9/10/2024.
//

#pragma once

#include <SDL.h>

#define HYLOG_DEBUG(fmt, ...) ::SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, fmt, ## __VA_ARGS__)
#define HYLOG_INFO(fmt, ...) ::SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, fmt, ## __VA_ARGS__)
#define HYLOG_WARN(fmt, ...) ::SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, fmt, ## __VA_ARGS__)
#define HYLOG_ERROR(fmt, ...) ::SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, fmt, ## __VA_ARGS__)