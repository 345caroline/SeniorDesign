#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from gnuradio import gr, analog

class BluetoothSweepingJammer(gr.top_block):
    def __init__(self, samp_rate=1e6, center_freq=2.45e9, freq_range=1e6):
        gr.top_block.__init__(self, "Bluetooth Sweeping Jammer")

        # Blocks
        self.source = analog.sig_source_c(samp_rate, analog.GR_SIN_WAVE, 0, 1, 0)
        self.mixer = analog.sig_mix_c(samp_rate, 1, center_freq)
        self.low_pass_filter = gr.firdes.low_pass(1, samp_rate, freq_range / 2, freq_range / 20)
        self.low_pass_filter_block = gr.freq_xlating_fir_filter_ccc(1, (self.low_pass_filter,), 0, samp_rate)
        self.amplitude = analog.sig_source_c(samp_rate, analog.GR_CONST_WAVE, 0.5, 0, 0)

        # Connections
        self.connect(self.source, self.mixer, self.low_pass_filter_block, self.amplitude)

        # Set input and output
        self.source.set_sampling_freq(samp_rate)
        self.amplitude.set_sampling_freq(samp_rate)

def main():
    # Set the center frequency and frequency range for Bluetooth (2.4 GHz to 2.5 GHz)
    center_freq = 2.45e9
    freq_range = 1e6
    tb = BluetoothSweepingJammer(center_freq=center_freq, freq_range=freq_range)
    tb.start()
    tb.wait()

if __name__ == '__main__':
    main()
