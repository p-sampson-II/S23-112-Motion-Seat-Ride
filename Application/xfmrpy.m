/*function [l1,l2,l3,l4,l5,l6] = fcn(roll,pitch,yaw)
    b=[-1.75 12.63 0; 1.75 12.63 0; 11.81 -4.8 0; 10.06 -7.83 0; -10.06 -7.83 0; -11.81 -4.8 0];
    p=[-8.1 6.69 0; 8.1 6.69 0; 9.84 3.67 0; 1.74 -10.35 0; -1.74 -10.35 0; -9.84 3.67 0];


    Rx=[1 0 0; 0 cos(roll) -sin(roll); 0 sin(roll) cos(roll)];
    Ry=[cos(pitch) 0 sin(pitch); 0 1 0; -sin(pitch) 0 cos(pitch)];
    Rz=[cos(yaw) -sin(yaw) 0; sin(yaw) cos(yaw) 0; 0 0 1];
    R=Rx*Ry*Rz;

    T=[0;0;23];

    l = zeros([3,6]);
    for i = 1:6
        p_i = transpose(p(i,:));
        b_i = transpose(b(i,:));
        l(:,i) =T + R*p_i-b_i;
    end
    l1=norm(l(1,));
    l2=norm(l(2));
    l3=norm(l(3));
    l4=norm(l(4));
    l5=norm(l(5));
    l6=norm(l(6));
end
*/