#include "algorithm.h"
#include <stdio.h>

// Tamanho do buffer para o cálculo da média móvel
const int32_t MA_SIZE = 4;

// Buffers para os dados
static int32_t an_ir_buffer[BUFFER_SIZE];
static int32_t an_red_buffer[BUFFER_SIZE];

void calc_spo2_and_hr(int32_t *pn_spo2, int8_t *pch_spo2_valid, int32_t *pn_heart_rate, int8_t *pch_hr_valid, uint32_t *pun_ir_buffer, uint32_t *pun_red_buffer)
{
    uint32_t un_ir_mean, un_red_mean;
    int32_t i, n_i_ratio_count;
    int32_t n_exact_ir_ac_sq_sum, n_exact_red_ac_sq_sum;
    int32_t n_middle_idx;
    int32_t n_spo2_calc;
    int32_t n_y_ac, n_x_ac;
    int32_t n_spo2_r_val;
    int32_t n_peaks_ir;
    int32_t an_ir_valley_locs[15];
    int32_t n_peak_interval_sum;

    // Calcula a média DC
    un_ir_mean = 0;
    un_red_mean = 0;
    for (i = 0; i < BUFFER_SIZE; i++) {
        un_ir_mean += pun_ir_buffer[i];
        un_red_mean += pun_red_buffer[i];
    }
    un_ir_mean = un_ir_mean / BUFFER_SIZE;
    un_red_mean = un_red_mean / BUFFER_SIZE;

    // Remove o componente DC e calcula o componente AC
    n_exact_ir_ac_sq_sum = 0;
    n_exact_red_ac_sq_sum = 0;
    for (i = 0; i < BUFFER_SIZE; i++) {
        n_y_ac = pun_ir_buffer[i] - un_ir_mean;
        n_x_ac = pun_red_buffer[i] - un_red_mean;
        n_exact_ir_ac_sq_sum += n_y_ac * n_y_ac;
        n_exact_red_ac_sq_sum += n_x_ac * n_x_ac;
    }

    // Calcula o valor R para o SpO2
    n_spo2_r_val = (n_exact_red_ac_sq_sum / un_red_mean) * (un_ir_mean) / (n_exact_ir_ac_sq_sum) ;
    n_spo2_calc = -45.060 * n_spo2_r_val * n_spo2_r_val / 10000 + 30.354 * n_spo2_r_val / 100 + 94.845;
    
    *pn_spo2 = n_spo2_calc;
    *pch_spo2_valid = (*pn_spo2 > 80 && *pn_spo2 < 100);

    // Encontra picos no sinal IR para calcular a frequência cardíaca
    for(i=0; i < BUFFER_SIZE; i++) an_ir_buffer[i] = (int32_t)pun_ir_buffer[i];
    find_peaks(an_ir_valley_locs, &n_peaks_ir, an_ir_buffer, BUFFER_SIZE, 30, 15);
    
    if (n_peaks_ir >= 2) {
        n_peak_interval_sum = 0;
        for (i = 1; i < n_peaks_ir; i++) {
            n_peak_interval_sum += (an_ir_valley_locs[i] - an_ir_valley_locs[i - 1]);
        }
        n_peak_interval_sum = n_peak_interval_sum / (n_peaks_ir - 1);
        *pn_heart_rate = (int32_t)((FS * 60) / n_peak_interval_sum);
        *pch_hr_valid = (*pn_heart_rate > 30 && *pn_heart_rate < 200);
    } else {
        *pn_heart_rate = 0;
        *pch_hr_valid = false;
    }
}

void find_peaks(int32_t *pun_locs, int32_t *pn_npks, int32_t *pun_x, int32_t n_size, int32_t n_min_height, int32_t n_min_distance)
{
    int32_t i = 1, n_width;
    *pn_npks = 0;

    while (i < n_size - 1) {
        if (pun_x[i] > n_min_height && pun_x[i] > pun_x[i-1] && pun_x[i] > pun_x[i+1]) {
            n_width = 1;
            while(i + n_width < n_size-1 && pun_x[i] == pun_x[i+n_width])
                n_width++;
            if (*pn_npks > 0 && (i - pun_locs[*pn_npks - 1]) < n_min_distance) {
                if(pun_x[i] > pun_x[pun_locs[*pn_npks - 1]])
                    pun_locs[*pn_npks - 1] = i;
            } else {
                 pun_locs[*pn_npks] = i;
                 (*pn_npks)++;
            }
            i += n_width;
        } else {
            i++;
        }
    }
}