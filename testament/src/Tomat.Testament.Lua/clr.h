#pragma once

#ifdef __cplusplus
extern "C" bool get_clr_initialized();
extern "C" void start_clr();
#else
bool get_clr_initialized(void);
void start_clr(void);
#endif
