% example
clear all;

% create master cube
a = master_cube

% check who's connected with master sub cube
fprintf('master sub cube:\n');
m_response = a.master()

% check who's connected with level 1 cubes
for n = 1:length(m_response.connections)
    if m_response.connections(n) == 1
        fprintf('slave level 1 - %s:\n',m_response.who(n))
        
        s1_response(n) = a.slave(m_response.who(n));
        s1_response(n)
    end
end

% check who's connected with level 2 cubes
for s = 1:length(s1_response)
    for n = 1:length(s1_response(s).connections)
        if s1_response(s).connections(n) == 1
            fprintf('slave level 2 - %s:\n',s1_response(s).who(n))
            
            s2_response(s,n) = a.slave(m_response.who(n));
            s2_response(s,n)
        end
    end
end