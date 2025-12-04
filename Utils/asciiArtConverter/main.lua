
---@return string
local function readFile()

    local file = io.open("input.txt", "r")

    if file == nil then
        print("ERROR: couldnt find open input.txt")
        return ""
    end

    --return data
    local data = file:read("a")    
    
    file:close()

    return data
end

---@param data table
local function writeFile(data)
    local file = io.open("output.txt", "w")

    if file == nil then
        print("ERROR: couldnt find open input.txt")
        return
    end

    file:write(".db ")
    
    for i = 1, #data, 1 do
        
        file:write("0x")
        file:write(data[i])
        file:write(",")

        if i % 8 == 0 then
            file:write("\n")
        end
    end
    
    file:close()    
end

---Converts each character in a string to its ASCII value
---@param str string
---@return table
local function strToByte(str)
    local output = {}

    for i = 1, #str, 1 do
        table.insert(output, str:byte(i))        
    end    

    print("------------------")
    print(#output .. "bytes")
    print(#output / 1024 .. "kb")
    print("------------------")
    
    return output
end


print("Running.....")
local asciiText = readFile()
local byteArr = strToByte(asciiText)
writeFile(byteArr)
print("Closing.....")





