%% Connect to the EyeTribe system

eyetribe('connect', '10.211.55.3', '6555');

%% Get some frames and draw them to the screen
% Use ctrl-C to exit

buffer = zeros(100, 2);
i = 0;

while(1)
    sample = eyetribe('get_frame'); 
    disp(sample.raw);
    pause(0.1);
    
    i = i + 1;
    if(i > 100), i = 1; end;
    buffer(i, 1) = sample.raw.x;
    buffer(i, 2) = sample.raw.y;
    
    clf; plot(buffer(:, 1), -buffer(:, 2), 'x');
    drawnow();
end

%% Disconnect from EyeTribe

eyetribe('disconnect');
