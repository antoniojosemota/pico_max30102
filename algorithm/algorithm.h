#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <stdint.h>

#define true 1
#define false 0

#define FS 100         // Frequência de amostragem
#define BUFFER_SIZE (FS * 4) // Buffer para 4 segundos de dados

void find_peaks(int32_t *pun_locs, int32_t *pn_npks, int32_t *pun_x, int32_t n_size, int32_t n_min_height, int32_t n_min_distance);
void calc_spo2_and_hr(int32_t *pn_spo2, int8_t *pch_spo2_valid, int32_t *pn_heart_rate, int8_t *pch_hr_valid, uint32_t *pun_ir_buffer, uint32_t *pun_red_buffer);

#endif /* ALGORITHM_H_ */