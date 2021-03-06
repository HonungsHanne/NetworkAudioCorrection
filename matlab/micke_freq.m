clearvars

[x, fsx] = audioread('before.wav');
[y, fsy] = audioread('after.wav');

% cut silence
x = x(fsx * 2 : fsx * 3.5);
y = y(fsy * 2 : fsy * 3.5);

% create spectrum
[Px, fx] = pwelch(x, [], [], length(x), fsx);
[Py, fy] = pwelch(y, [], [], length(y), fsy);
powPx = pow2db(Px);
powPy = pow2db(Py);

xLin = 1:0.01:4.5;
for i = 1:length(xLin)
    xLog(i) = round(10^xLin(i));
end

for i = 2:length(xLog)-1
   newPowPx(i) = mean(powPx(xLog(i-1):xLog(i+1))); 
end

fx = fx(xLog(2:length(xLog)));
powPx = newPowPx;

for i = 2:length(xLog)-1
   newPowPy(i) = mean(powPy(xLog(i-1):xLog(i+1))); 
end

fy = fy(xLog(2:length(xLog)));
powPy = newPowPy;

min_total = min([min(powPx), min(powPy)]) - 20;
max_total = max([max(powPx(2:length(powPx))), max(powPy(2:length(powPx)))]) + 20;

x_mean = mean(powPx);
y_mean = mean(powPy);

ax = subplot(2, 1, 1);
plot(ax, fx, powPx);
axis(ax, [44, 22720, min_total, max_total]);
set(ax, 'XScale', 'log')
title(ax, 'Before');
ylabel(ax, 'Magnitude (dB)');
xlabel(ax, 'Frequency (Hz)');
grid on

ay = subplot(2, 1, 2);
plot(ay, fy, powPy);
axis(ay, [44, 22720, min_total, max_total]);
set(ay, 'XScale', 'log')
title(ay, 'After');
ylabel(ay, 'Magnitude (dB)');
xlabel(ay, 'Frequency (Hz)');
grid on