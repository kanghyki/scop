#include "logger.h"
#include "obj_loader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

bool LoadObject(const std::string &in_filepath, Object &out_obj)
{
    auto ext_s = in_filepath.find_last_of('.');
    if (ext_s == std::string::npos || in_filepath.substr(ext_s) != ".obj")
    {
        logger::warning << "::LOAD_OBJECT::UNSUPPORTED_FILE_EXTENSION" << logger::endl;
        return false;
    }

    std::ifstream ifs(in_filepath);
    if (!ifs.is_open())
    {
        logger::warning << "::LOAD_OBJECT::FAILED_TO_OPEN_FILE" << logger::endl;
        return false;
    }

    ftm::vec3 maxv, minv;

    std::string line;
    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        std::string head;
        ss >> head;

        if (head == "#")
        {
            continue;
        }
        else if (head == "o")
        {
            ss >> out_obj.name_;
        }
        else if (head == "v")
        {
            ftm::vec3 v;
            if (ss >> v.x >> v.y >> v.z)
            {
                maxv.x = std::max(maxv.x, v.x);
                maxv.y = std::max(maxv.y, v.y);
                maxv.z = std::max(maxv.z, v.z);

                minv.x = std::min(minv.x, v.x);
                minv.y = std::min(minv.y, v.y);
                minv.z = std::min(minv.z, v.z);

                out_obj.center_ = out_obj.center_ + v;

                out_obj.vpos_.push_back(v);
            }
        }
        else if (head == "vt")
        {
            ftm::vec2 v;
            if (ss >> v.x >> v.y)
            {
                out_obj.vuv_.push_back(v);
            }
        }
        else if (head == "vn")
        {
            ftm::vec3 v;
            if (ss >> v.x >> v.y >> v.z)
            {
                out_obj.vnorm_.push_back(v);
            }
        }
        else if (head == "f")
        {
            std::vector<uint32_t> temp[3];
            while (!ss.eof())
            {
                ss >> std::ws;
                int i = -1;
                do
                {
                    ++i;
                    if (!std::isdigit(ss.peek()))
                    {
                        continue;
                    }
                    size_t n;
                    ss >> n;
                    temp[i].push_back(n);
                } while (i < 3 && ss.peek() == '/' && ss.get());
            }
            std::vector<uint32_t> &vi = temp[0];
            std::vector<uint32_t> &ui = temp[1];
            std::vector<uint32_t> &ni = temp[2];
            for (int i = 0; i + 2 < temp[0].size(); ++i)
            {
                out_obj.ipos_.push_back(vi[0] - 1);
                out_obj.ipos_.push_back(vi[i + 1] - 1);
                out_obj.ipos_.push_back(vi[i + 2] - 1);

                if (i + 2 < ni.size())
                {
                    out_obj.inorm_.push_back(ni[0] - 1);
                    out_obj.inorm_.push_back(ni[i + 1] - 1);
                    out_obj.inorm_.push_back(ni[i + 2] - 1);
                }

                if (i + 2 < ui.size())
                {
                    out_obj.iuv_.push_back(ui[0] - 1);
                    out_obj.iuv_.push_back(ui[i + 1] - 1);
                    out_obj.iuv_.push_back(ui[i + 2] - 1);
                }
            }
        }
        else if (head == "usemtl" || head == "g" || head == "s")
        {
            continue;
        }
        // Unknown tags are ignored
    }
    ifs.close();

    // Post-processing
    if (!out_obj.vpos_.empty())
    {
        out_obj.center_ = out_obj.center_ / (float)out_obj.vpos_.size();
    }

    out_obj.vertices_.reserve(out_obj.ipos_.size());
    for (int i = 0; i + 2 < out_obj.ipos_.size(); i += 3)
    {
        Vertex v0, v1, v2;
        v0.position = out_obj.vpos_[out_obj.ipos_[i]];
        v1.position = out_obj.vpos_[out_obj.ipos_[i + 1]];
        v2.position = out_obj.vpos_[out_obj.ipos_[i + 2]];
        if (i + 2 < out_obj.inorm_.size())
        {
            v0.normal = out_obj.vnorm_[out_obj.inorm_[i]];
            v1.normal = out_obj.vnorm_[out_obj.inorm_[i + 1]];
            v2.normal = out_obj.vnorm_[out_obj.inorm_[i + 2]];
        }
        else
        {
            auto p0 = v1.position - v0.position;
            auto p1 = v2.position - v0.position;
            auto n0 = ftm::normalize(ftm::cross(p0, p1));
            v0.normal = n0;
            v1.normal = n0;
            v2.normal = n0;
        }
        if (i + 2 < out_obj.iuv_.size())
        {
            v0.uv = out_obj.vuv_[out_obj.iuv_[i]];
            v1.uv = out_obj.vuv_[out_obj.iuv_[i + 1]];
            v2.uv = out_obj.vuv_[out_obj.iuv_[i + 2]];
        }
        else
        {
            auto CalcUV = [&](const ftm::vec3 &v) -> ftm::vec2 {
                ftm::vec2 uv;
                // Basic spherical mapping
                uv.x = (atan2(v.z, v.x) / M_PI + 1.0f) / 2.0f;
                // Linear mapping based on Y bounds
                uv.y = (v.y - minv.y) / (maxv.y - minv.y);

                return uv;
            };
            v0.uv = CalcUV(v0.position);
            v1.uv = CalcUV(v1.position);
            v2.uv = CalcUV(v2.position);
        }

        out_obj.vertices_.push_back(v0);
        out_obj.vertices_.push_back(v1);
        out_obj.vertices_.push_back(v2);
    }

    for (int i = 0; i + 2 < out_obj.vertices_.size(); i += 3)
    {
        out_obj.indices_.push_back(i);
        out_obj.indices_.push_back(i + 1);
        out_obj.indices_.push_back(i + 2);
    }

    return true;
}
