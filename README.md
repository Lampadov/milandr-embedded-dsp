# CMSIS-DSP и аппаратный CORDIC на микроконтроллерах Миландр

В первом примере ищем частоту синусоиды и разбираемся, какие таблицы FFT действительно нужны в прошивке. Во втором отдаём тригонометрию аппаратному блоку К1986ВК01GI и сравниваем результат с эталоном из `math.h`.

## FFT на К1986ВЕ1QI

Возьмём синусоиду **333 Гц**. В [main.c](examples/cmsis-fft/main.c) формируем 1 024 отсчёта при частоте дискретизации 1 024 Гц. На выходе `arm_rfft_fast_f32()` получаем спектр. Программа считает модули и ищет максимум. Шаг между частотными бинами составляет 1 Гц, поэтому в отладчике ожидаем **`maxIndex = 333`**. Массив `fft_power` содержит модули спектра. В `check` сохраняется показание TIMER4 после вызова FFT.

В [проекте Keil](examples/cmsis-fft/FFT.uvprojx) заданы макросы CMSIS-DSP для таблиц RFFT длиной 1 024 и используемого внутри FFT длиной 512. Откройте map-файл сборки, чтобы проверить, какие таблицы попали в образ.

Для этой конфигурации нужны **Arm Compiler 5.06**, `Milandr.MDR32FxQI.1.0`, CMSIS Core 5.5.0 и DSP 1.9.0-dev. Скопируйте `RTE/Device/` из оригинального `FFT.zip` в [каталог примера](examples/cmsis-fft/). [Какие макросы включить и где взять архив][fft].

## CORDIC на К1986ВК01GI

В [этом примере](examples/cordic/main.c) синус и косинус считаются в целочисленном режиме, Float32 и Float16. Затем в отдельном проходе CORDIC определяет угол и модуль вектора. Для каждой серии программа печатает результат, эталон и ошибку. В конце серии можно посмотреть максимальное отклонение.

Откройте [cordic.uvprojx](examples/cordic/cordic.uvprojx) с **Arm Compiler 6** и пакетом `Keil.MDR1986VK01.1.3`. В исходном проекте указан компилятор 6.10.1. Проверьте компоненты **Startup**, **CMSIS Core** и **Compiler I/O** в **Manage Run-Time Environment**. У проекта есть цели **FLASH** и **RAM**. При запуске из Flash для вывода `printf()` настройте ITM Trace, как показано в статье. [Режимы CORDIC и вывод отладчика][cordic].

Обе статьи написал **Илья Лампадов** для портала Миландр (Milandr). [CMSIS-DSP][fft] · [CORDIC][cordic].

[fft]: https://support.milandr.ru/base/primenenie/programmirovanie-32-razryadnykh-mk/osobennosti-primeneniya-know-how/55372/
[cordic]: https://support.milandr.ru/base/primenenie/programmirovanie-32-razryadnykh-mk/osobennosti-primeneniya-know-how/24230/
