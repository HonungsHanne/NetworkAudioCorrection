[x, fsx] = audioread('before.wav');
[y, fsy] = audioread('after.wav');

% cut silence
x = x(fs * 2 : fs * 3.5);
y = y(fs * 2 : fs * 3.5);

% create spectrum
[Px, fx] = pwelch(x, [], [], length(x), fsx);
[Py, fy] = pwelch(y, [], [], length(y), fsy);
powPx = pow2db(Px);
powPy = pow2db(Py);

min_total = min([min(powPx), min(powPy)]);
max_total = max([max(powPx), max(powPy)]);

ax = subplot(2, 1, 1);
plot(ax, fx, powPx);
axis(ax, [44, 22720, min_total, max_total]);
set(ax, 'XScale', 'log')
title(ax, 'Before');
ylabel(ax, 'Magnitude (dB)');
xlabel(ax, 'Frequency (Hz)');

ay = subplot(2, 1, 2);
plot(ay, fy, powPy);
axis(ay, [44, 22720, min_total, max_total]);
set(ay, 'XScale', 'log')
title(ay, 'After');
ylabel(ay, 'Magnitude (dB)');
xlabel(ay, 'Frequency (Hz)');