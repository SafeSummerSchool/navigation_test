clear,close all; clc;


% ____________________________
%               xx
%               xx
% o                 xx
%                   xx
% ____________________________

width = 20; % Width of field in meters
length = 40; % Length of field in meters
gridSize = 0.1; % Defines resolution of grid. 



robotOrigo = [0; 0; 1];
robotDirection = [1; 0; 1];
objects = [2 4; -2 2; 1 1];
objectSizes = [1.5 2];
objectSizes = objectSizes/gridSize; 
line1 = [-4 5;-1 5; 1 1];
line2 = [-4 5;-5 1; 1 1];

theta = atan(diff(line1(2,:))/diff(line1(1,:)));
thetaDeg = (180/pi)*theta;
T1 = [ cos(theta) sin(theta) 0;
      -sin(theta) cos(theta) 0;
                0          0 1];

% In c,c++ or python remove the +1.
T2 = [1/gridSize          0                       +1;
               0 1/gridSize     1/gridSize*width/2+1;
               0          0                        1];

figure; 
plot(robotOrigo(1),robotOrigo(2),'o');
hold on; plot(objects(1,:),objects(2,:),'x');
hold on; plot([robotOrigo(1) robotDirection(1)],[robotOrigo(2) robotDirection(2)]);
hold on; plot(line1(1,:),line1(2,:))
hold on; plot(line2(1,:),line2(2,:))
grid on; axis equal


robotOrigo_2 = T2*T1*robotOrigo;
robotDirection_2 = T2*T1*robotDirection;
line1_2 = T2*T1*line1;
line2_2 = T2*T1*line2;
objects_2 = T2*T1*objects;

figure; 
plot(robotOrigo_2(1),robotOrigo_2(2),'o');
hold on; plot([robotOrigo_2(1) robotDirection_2(1)],[robotOrigo_2(2) robotDirection_2(2)]);
hold on; plot(objects_2(1,:),objects_2(2,:),'x');
hold on; plot(line1_2(1,:),line1_2(2,:));
hold on; plot(line2_2(1,:),line2_2(2,:));
grid on; axis equal

map = zeros(width/gridSize,length/gridSize);

% % Draw objects
% for n = 1:size(objects_2,2)
%     %map(round(objects_2(2,n)),round(objects_2(1,n))) = 1;
% %     for nn = 1:
%     map(round(objects_2(2,n)),round(objects_2(1,n))) = 1;
% end

% Draw objects
for n = 1:size(objects_2,2)
    for nn = 1:objectSizes(n)
        for nnn = 1:objectSizes(n)
            r = round(objects_2(2,n)-objectSizes(n)/2+nn);
            c = round(objects_2(1,n)-objectSizes(n)/2+nnn);
            % This lines should also be correct in c,c++ and python.
            if(r > 0 && r <= size(map,1) && c > 0 && c <= size(map,2))
                map(r,c) = 1;
            end
        end
    end
end



% Draw lines
map(round(line1_2(2,1)),:) = 1;
map(round(line2_2(2,1)),:) = 1;
figure();
imshow(map)
hold on
plot(round(robotOrigo_2(1)),round(robotOrigo_2(2)),'ro');


% Draw robot
% map(,round(robotOrigo_2(1))) = 1;
