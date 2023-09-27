#pragma once
#include <opencv2/core.hpp>

/**
 * @brief Realiza una combinación "hard" entre dos imágenes usando una máscara.
 * @param foreground la imagen "primer plano".
 * @param background la imagen "fondo".
 * @param mask la máscara 0 (fondo) / 255 (primer plano).
 * @return la imagen resultante de la combinación.
 */
cv::Mat fsiv_combine_images(const cv::Mat& foreground, const cv::Mat& background,
                       const cv::Mat& mask);


/**
 * @brief Crea una máscara para marcar puntos dentro de un rango HSV.
 * @param img es la imagen de entrada (BGR).
 * @param lower_bound es el límite inferior del rango HSV.
 * @param upper_bound es el límite superior del rango HSV.
 * @return la máscara (0/255).
 */
cv::Mat fsiv_create_mask_from_hsv_range(const cv::Mat& img,
                                        const cv::Scalar& lower_bound,
                                        const cv::Scalar& upper_bound);


/**
 * @brief Sustituye en fondo de una imagen por otra usando un color clave.
 * @param foreg imagen que representa el primer plano.
 * @param backg imagen que representa el fondo con el que rellenar.
 * @param hue tono del color usado como color clave.
 * @param sensitivity permite ampliar el rango de tono con hue +- sensitivity.
 * @return la imagen con la la composición.
 */
cv::Mat fsiv_apply_chroma_key(const cv::Mat &foreg, const cv::Mat& backg, int hue,
                           int sensitivity);
