#pragma once

struct Url
{
public:
	std::wstring QueryString, Path, Protocol, Host, Port;

	inline friend std::ostream& operator << (std::ostream& stream, Url url) {
		auto host = std::string(url.Host.begin(), url.Host.end());
		stream << host << '\n';
		auto port = std::string(url.Port.begin(), url.Port.end());
		stream << port << '\n';
		auto protocol = std::string(url.Protocol.begin(), url.Protocol.end());
		stream << protocol << '\n';
		auto path = std::string(url.Path.begin(), url.Path.end());
		stream << path << '\n';
		auto qs = std::string(url.QueryString.begin(), url.QueryString.end());
		stream << qs << '\n';

		return stream;
	}

	static Url Parse(const std::wstring& uri)
	{
		Url result;
		typedef std::wstring::const_iterator iterator_t;

		if (uri.length() == 0)
			return result;

		iterator_t uriEnd = uri.end();

		iterator_t queryStart = std::find(uri.begin(), uriEnd, L'?');

		iterator_t protocolStart = uri.begin();
		iterator_t protocolEnd = std::find(protocolStart, uriEnd, L':');

		if (protocolEnd != uriEnd)
		{
			std::wstring prot = &*(protocolEnd);
			if ((prot.length() > 3) && (prot.substr(0, 3) == L"://"))
			{
				result.Protocol = std::wstring(protocolStart, protocolEnd);
				protocolEnd += 3;
			}
			else
				protocolEnd = uri.begin();
		}
		else
			protocolEnd = uri.begin();

		iterator_t hostStart = protocolEnd;
		iterator_t pathStart = std::find(hostStart, uriEnd, L'/');

		iterator_t hostEnd = std::find(protocolEnd, (pathStart != uriEnd) ? pathStart : queryStart, L':');

		result.Host = std::wstring(hostStart, hostEnd);

		if ((hostEnd != uriEnd) && ((&*(hostEnd))[0] == L':'))
		{
			hostEnd++;
			iterator_t portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
			result.Port = std::wstring(hostEnd, portEnd);
		}

		if (pathStart != uriEnd)
			result.Path = std::wstring(pathStart, queryStart);

		if (queryStart != uriEnd)
			result.QueryString = std::wstring(queryStart, uri.end());

		return result;

	}
};
