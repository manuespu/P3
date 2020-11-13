cd('C:\Users\manue\Desktop\PAV\P3\scripts')
[y,Fs] = audioread('voiced.wav');
deltat=1/Fs;
t=0:deltat:(length(y)*deltat)-deltat;
subplot(2,1,1);
plot(t,y); title('Vocal A'); xlabel( 'Seconds'); ylabel('Amplitude');
subplot(2,1,2);
c = xcorr(y);
plot(c); title('Autocorrelation');  xlabel( 'Samples'); ylabel('Amplitude');