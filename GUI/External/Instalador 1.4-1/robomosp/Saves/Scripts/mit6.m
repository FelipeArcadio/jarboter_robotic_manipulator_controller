% AJB 3-30-2001

function mitsuikine= mit6(mitsu,T)

a2 = mitsu(2,2);	%250
a3 = mitsu(3,2);	%160
d1 = mitsu(1,4);	%300 (altura de base puede tomarse en 0)
d5 = mitsu(5,4);	%dimensión efector final

n=T(1:3,1);
o=T(1:3,2);
a=T(1:3,3);
p=T(1:3,4);
p(3)=p(3)-d1;

%	P muñeca
p
p=p-d5*a

% 	Theta 1
theta(1) = atan2(p(2),p(1))

if (theta(1)<-150/180*pi ||theta(1)>150/180*pi)
    error('Teta1 fuera de rango');
end
%	distancia muñeca-hombro
R = sqrt(p(1)^2+p(2)^2+p(3)^2);

%	Angulo hombro
beta=abs(atan2(p(3),sqrt(p(1)^2+p(2)^2)))
sigma=abs(acos((R^2+a2^2-a3^2)/(2*a2*R)))
%if R <= (a2^2+a3^2) 
if p(3)<=0   
    theta(2)=sigma-beta
else
    theta(2)=beta+sigma 
end

if (theta(2)<-30/180*pi ||theta(2)>100/180*pi)
    error('Teta2 fuera de rango');
end

%	Angulo theta(3)
%	Ojo, este ángulo es siempre negativo en el mitsubishi
    theta(3) = -real(acos((R^2-a2^2-a3^2)/(2*a2*a3))) 
    
if (theta(3)<-110/180*pi)
    error('Teta3 fuera de rango');
end   
    
if R > (a2+a3) 
    error('La muñeca esta mas alla de donde podria estar en el RV-M1'); 
end

%	sen(t4)=a*x4,cos(t4)=-a*y4
T1=fkine(mitsu(1:3,:),[theta(1) theta(2) theta(3)]');
theta(4)=atan2(a'*T1(1:3,1),-a'*T1(1:3,2));

if (theta(4)<-pi ||theta(4)>pi)
    error('Teta4 fuera de rango');
end

T1=fkine(mitsu(1:4,:),[theta(1),theta(2),theta(3),theta(4)]');
%	sen(t5)=n*y5,cos(t5)=o*y5
theta(5) = atan2(n'*T1(1:3,2),o'*T1(1:3,2));


mitsuikine=theta;


