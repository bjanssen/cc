
classdef master_cube
    
    properties
        % master code
        master_code_order % order of cubes
        master_code_orientation_1 % orientation of output 1
        master_code_orientation_2 % orientation of output 2
        n_cubes % total nr of cubes
        
        % current state
        order % order of cubes
        orientation_1 % orientation of output 1
        orientation_2 % orientation of output 2
        
        % correct parameters
        correct_order
        correct_orientation_1
        correct_orientation_2
    end
    
    methods
        function obj = master_cube()
            % initialize master code
            
            % for now pick fixed code
            
            % order of cubes: M = front-top-right (always)
            % next three are top plane clock-wise
            % next four are bottom plane clock wise (starting with cube underneath master sub cube)
            obj.master_code_order = cube_label([0 1 2 3 4 5 6 7]);
            obj.n_cubes = length(obj.master_code_order);
            
            % orientation of output 1 and 2 (outputs are ordered like the numbers on a dice)
            % 'U' = up, 'D' = down, 'F' = front, 'B' = back, 'L' = left, 'R' = right
            obj.master_code_orientation_1 = ...
                [ cube_orientation.D, cube_orientation.U, cube_orientation.U, cube_orientation.F, ...
                cube_orientation.R, cube_orientation.D, cube_orientation.B, cube_orientation.F ];
            obj.master_code_orientation_2 = ...
                [ cube_orientation.L, cube_orientation.L, cube_orientation.F, cube_orientation.R, ...
                    cube_orientation.U, cube_orientation.R, cube_orientation.U, cube_orientation.R];
            
            % shuffle master cube
            obj = master_shuffle(obj);
        end
        
        function obj = master_shuffle(obj)
            % random start configuration master cube
            
            % order of cubes
            %obj.order = cube_label([0 randperm(7)]);
            obj.order = cube_label([0 1 2 3 4 5 6 7]);
            
            % orientation of cubes
            
            % init
            for n = 1:obj.n_cubes
                obj.orientation_1(n) = cube_orientation.D;
                obj.orientation_2(n) = cube_orientation.L;
            end
            % spin the cubes
            for n = 1:obj.n_cubes
                obj = spin(obj,n);
            end
        end
        
        function obj = spin(obj, cube_index)
            % create random orientation for a single cube
            
            if cube_index == 1
                % first cube is fixed
                obj.orientation_1(1) = cube_orientation.D;
                obj.orientation_2(1) = cube_orientation.L;
            else
                % return random cube orientation
                obj.orientation_1(cube_index) = cube_orientation(randperm(6,1));
                if obj.orientation_1(cube_index) == cube_orientation.U || obj.orientation_1(cube_index) == cube_orientation.D
                    orientations = cube_orientation([3 4 5 6]);
                    obj.orientation_2(cube_index) = orientations(randperm(4,1));
                elseif obj.orientation_1(cube_index) == cube_orientation.F || obj.orientation_1(cube_index) == cube_orientation.B
                    orientations = cube_orientation([1 2 5 6]);
                    obj.orientation_2(cube_index) = orientations(randperm(4,1));
                elseif obj.orientation_1(cube_index) == cube_orientation.L || obj.orientation_1(cube_index) == cube_orientation.R
                    orientations = cube_orientation([1 2 3 4]);
                    obj.orientation_2(cube_index) = orientations(randperm(4,1));
                end
            end
            
            % force enum (don't know why needed)
            obj.orientation_1 = cube_orientation(obj.orientation_1);
            obj.orientation_2 = cube_orientation(obj.orientation_2);
            
            % check cube
            obj = check(obj);
        end
        
        function obj = check(obj)
            % check state of master cube against master code
            obj.correct_order = obj.order == obj.master_code_order;
            obj.correct_orientation_1 = obj.orientation_1 == obj.master_code_orientation_1;
            obj.correct_orientation_2 = obj.orientation_2 == obj.master_code_orientation_2;
        end
        
        function obj = swap(obj, index1, index2)
            % swap out two cubes (keep the same orientation)
            
            if index1 == 1 || index2 == 1
                fprintf('Cannot touch cube M\n');
                return;
            end
            
            % swap
            tmp = obj.order(index1);
            obj.order(index1) = obj.order(index2);
            obj.order(index2) = tmp;

            % check cube
            obj = check(obj);
        end
        
        function obj = roll(obj, index)
            % roll cube (rotate clockwise over front-back axis)
            
            if index == 1
                fprintf('Cannot touch cube M\n');
                return;
            end
            
            % roll
            if obj.orientation_1(index) == cube_orientation.L
                obj.orientation_1(index) = cube_orientation.U;
            elseif obj.orientation_1(index) == cube_orientation.U
                obj.orientation_1(index) = cube_orientation.R;
            elseif obj.orientation_1(index) == cube_orientation.R
                obj.orientation_1(index) = cube_orientation.D;
            elseif obj.orientation_1(index) == cube_orientation.D
                obj.orientation_1(index) = cube_orientation.L;
            end
                
            if obj.orientation_2(index) == cube_orientation.L
                obj.orientation_2(index) = cube_orientation.U;
            elseif obj.orientation_2(index) == cube_orientation.U
                obj.orientation_2(index) = cube_orientation.R;
            elseif obj.orientation_2(index) == cube_orientation.R
                obj.orientation_2(index) = cube_orientation.D;
            elseif obj.orientation_2(index) == cube_orientation.D
                obj.orientation_2(index) = cube_orientation.L;
            end
                
            % check cube
            obj = check(obj);
        end
        
        function obj = yaw(obj, index)
            % yaw cube (rotate clockwise over top-bottom axis)
            
            if index == 1
                fprintf('Cannot touch cube M\n');
                return;
            end
            
            % roll
            if obj.orientation_1(index) == cube_orientation.L
                obj.orientation_1(index) = cube_orientation.B;
            elseif obj.orientation_1(index) == cube_orientation.B
                obj.orientation_1(index) = cube_orientation.R;
            elseif obj.orientation_1(index) == cube_orientation.R
                obj.orientation_1(index) = cube_orientation.F;
            elseif obj.orientation_1(index) == cube_orientation.F
                obj.orientation_1(index) = cube_orientation.L;
            end
                
            if obj.orientation_2(index) == cube_orientation.L
                obj.orientation_2(index) = cube_orientation.B;
            elseif obj.orientation_2(index) == cube_orientation.B
                obj.orientation_2(index) = cube_orientation.R;
            elseif obj.orientation_2(index) == cube_orientation.R
                obj.orientation_2(index) = cube_orientation.F;
            elseif obj.orientation_2(index) == cube_orientation.F
                obj.orientation_2(index) = cube_orientation.L;
            end
                
            % check cube
            obj = check(obj);
        end
        
        function response = master(obj)
            % response of master
            % #connections + who's connected
            
            % own label
            response.label = cube_label.M;
            
            % determine nr. of connections
            response.connections = zeros(1,obj.n_cubes);
            response.connections(1:3) = 1; % assume cube so all first three connections are there
            
            % determine who's connected
            response.who = cube_label(8*ones(1,obj.n_cubes));
            response.who(1) = obj.order(5);
            response.who(2) = obj.order(2);
            response.who(3) = obj.order(4);
        end
        
        function response = slave(obj,label)
            % response of slave
            
            response.label = label;
            
            % check where this cube is
            index = find(label == obj.order);
            if index == 1
                fprintf('Master is not a slave\n');
                response = [];
                return;
            end
                        
            % init nr. of connections
            response.connections = zeros(1,obj.n_cubes);
            response.who = [cube_label.x cube_label.x cube_label.x cube_label.x ...
                cube_label.x cube_label.x cube_label.x cube_label.x];
            
            % assume cube so three connections are there
            if index == 2
                % ports connected
                ports = [obj.getport(index,cube_orientation.D) obj.getport(index,cube_orientation.B) obj.getport(index,cube_orientation.R)]; % D B R
                response.connections(ports) = 1;
                % cubes connected to the ports
                response.who(ports) = obj.order([6 3 1]);
            elseif index == 3
                % ports connected
                ports = [obj.getport(index,cube_orientation.D) obj.getport(index,cube_orientation.R) obj.getport(index,cube_orientation.F)]; % D R F
                response.connections(ports) = 1;
                % cubes connected to the ports
                response.who(ports) = obj.order([7 4 2]);
            elseif index == 4
                % ports connected
                ports = [obj.getport(index,cube_orientation.D) obj.getport(index,cube_orientation.F) obj.getport(index,cube_orientation.L)]; % D F L
                response.connections(ports) = 1;
                % cubes connected to the ports
                response.who(ports) = obj.order([8 1 3]);
            elseif index == 5
                % ports connected
                ports = [obj.getport(index,cube_orientation.U) obj.getport(index,cube_orientation.L) obj.getport(index,cube_orientation.B)]; % U L B
                response.connections(ports) = 1;
                % cubes connected to the ports
                response.who(ports) = obj.order([1 6 8]);
            elseif index == 6
                % ports connected
                ports = [obj.getport(index,cube_orientation.U) obj.getport(index,cube_orientation.B) obj.getport(index,cube_orientation.R)]; % U B R
                response.connections(ports) = 1;
                % cubes connected to the ports
                response.who(ports) = obj.order([2 7 5]);
            elseif index == 7
                % ports connected
                ports = [obj.getport(index,cube_orientation.U) obj.getport(index,cube_orientation.R) obj.getport(index,cube_orientation.F)]; % U R F
                response.connections(ports) = 1;
                % cubes connected to the ports
                response.who(ports) = obj.order([3 8 6]);
            elseif index == 8
                % ports connected
                ports = [obj.getport(index,cube_orientation.U) obj.getport(index,cube_orientation.F) obj.getport(index,cube_orientation.L)]; % U F L
                response.connections(ports) = 1;
                % cubes connected to the ports
                response.who(ports) = obj.order([4 5 7]);
            end
        end
        
        function port = getport(obj,index,orientation)
            % returns which port is connected to orientation of cube with index
            
            if obj.orientation_1(index) == orientation
                port = 1;
            elseif obj.orientation_2(index) == orientation
                port = 2;
            elseif orientation == cube_orientation.U && obj.orientation_1(index) == cube_orientation.D
                port = 6;
            elseif orientation == cube_orientation.D && obj.orientation_1(index) == cube_orientation.U
                port = 6;
            elseif orientation == cube_orientation.L && obj.orientation_1(index) == cube_orientation.R
                port = 6;
            elseif orientation == cube_orientation.R && obj.orientation_1(index) == cube_orientation.L
                port = 6;
            elseif orientation == cube_orientation.F && obj.orientation_1(index) == cube_orientation.B
                port = 6;
            elseif orientation == cube_orientation.B && obj.orientation_1(index) == cube_orientation.F
                port = 6;
            elseif orientation == cube_orientation.U && obj.orientation_2(index) == cube_orientation.D
                port = 5;
            elseif orientation == cube_orientation.D && obj.orientation_2(index) == cube_orientation.U
                port = 5;
            elseif orientation == cube_orientation.L && obj.orientation_2(index) == cube_orientation.R
                port = 5;
            elseif orientation == cube_orientation.R && obj.orientation_2(index) == cube_orientation.L
                port = 5;
            elseif orientation == cube_orientation.F && obj.orientation_2(index) == cube_orientation.B
                port = 5;
            elseif orientation == cube_orientation.B && obj.orientation_2(index) == cube_orientation.F
                port = 5;
            elseif orientation == cube_orientation.U && obj.orientation_1(index) == cube_orientation.F && obj.orientation_2(index) == cube_orientation.R
                port = 4;
            elseif orientation == cube_orientation.U && obj.orientation_1(index) == cube_orientation.L && obj.orientation_2(index) == cube_orientation.F
                port = 4;
            elseif orientation == cube_orientation.U && obj.orientation_1(index) == cube_orientation.B && obj.orientation_2(index) == cube_orientation.L
                port = 4;
            elseif orientation == cube_orientation.U && obj.orientation_1(index) == cube_orientation.R && obj.orientation_2(index) == cube_orientation.B
                port = 4;
            elseif orientation == cube_orientation.D && obj.orientation_1(index) == cube_orientation.F && obj.orientation_2(index) == cube_orientation.L
                port = 4;
            elseif orientation == cube_orientation.D && obj.orientation_1(index) == cube_orientation.L && obj.orientation_2(index) == cube_orientation.B
                port = 4;
            elseif orientation == cube_orientation.D && obj.orientation_1(index) == cube_orientation.B && obj.orientation_2(index) == cube_orientation.R
                port = 4;
            elseif orientation == cube_orientation.D && obj.orientation_1(index) == cube_orientation.R && obj.orientation_2(index) == cube_orientation.F
                port = 4;
            elseif orientation == cube_orientation.L && obj.orientation_1(index) == cube_orientation.F && obj.orientation_2(index) == cube_orientation.U
                port = 4;
            elseif orientation == cube_orientation.L && obj.orientation_1(index) == cube_orientation.D && obj.orientation_2(index) == cube_orientation.F
                port = 4;
            elseif orientation == cube_orientation.L && obj.orientation_1(index) == cube_orientation.B && obj.orientation_2(index) == cube_orientation.D
                port = 4;
            elseif orientation == cube_orientation.L && obj.orientation_1(index) == cube_orientation.U && obj.orientation_2(index) == cube_orientation.B
                port = 4;
            elseif orientation == cube_orientation.R && obj.orientation_1(index) == cube_orientation.F && obj.orientation_2(index) == cube_orientation.D
                port = 4;
            elseif orientation == cube_orientation.R && obj.orientation_1(index) == cube_orientation.D && obj.orientation_2(index) == cube_orientation.B
                port = 4;
            elseif orientation == cube_orientation.R && obj.orientation_1(index) == cube_orientation.B && obj.orientation_2(index) == cube_orientation.U
                port = 4;
            elseif orientation == cube_orientation.R && obj.orientation_1(index) == cube_orientation.U && obj.orientation_2(index) == cube_orientation.F
                port = 4;
            elseif orientation == cube_orientation.F && obj.orientation_1(index) == cube_orientation.L && obj.orientation_2(index) == cube_orientation.D
                port = 4;
            elseif orientation == cube_orientation.F && obj.orientation_1(index) == cube_orientation.U && obj.orientation_2(index) == cube_orientation.L
                port = 4;
            elseif orientation == cube_orientation.F && obj.orientation_1(index) == cube_orientation.R && obj.orientation_2(index) == cube_orientation.U
                port = 4;
            elseif orientation == cube_orientation.F && obj.orientation_1(index) == cube_orientation.D && obj.orientation_2(index) == cube_orientation.R
                port = 4;
            elseif orientation == cube_orientation.B && obj.orientation_1(index) == cube_orientation.L && obj.orientation_2(index) == cube_orientation.U
                port = 4;
            elseif orientation == cube_orientation.B && obj.orientation_1(index) == cube_orientation.U && obj.orientation_2(index) == cube_orientation.R
                port = 4;
            elseif orientation == cube_orientation.B && obj.orientation_1(index) == cube_orientation.R && obj.orientation_2(index) == cube_orientation.D
                port = 4;
            elseif orientation == cube_orientation.B && obj.orientation_1(index) == cube_orientation.D && obj.orientation_2(index) == cube_orientation.L
                port = 4;
            else
                port = 3;
            end
            
        end
        
    end % methods
    
end % class
   
